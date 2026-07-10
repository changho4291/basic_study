#include "RtspRelayServer.h"

#include <glib.h>

#include <iostream>
#include <sstream>
#include <utility>

RtspRelayServer::RtspRelayServer(std::string service)
    : service_(std::move(service)) {}

RtspRelayServer::~RtspRelayServer() {
    stop();
}

bool RtspRelayServer::add_h264_camera(const CameraConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (running_) {
        std::cerr << "RtspRelayServer: cannot add camera while server is running. "
                  << "Stop server first. camera_id=" << config.camera_id
                  << std::endl;
        return false;
    }

    if (config.camera_id.empty()) {
        std::cerr << "RtspRelayServer: camera_id is empty" << std::endl;
        return false;
    }

    if (config.rtsp_uri.empty()) {
        std::cerr << "RtspRelayServer: rtsp_uri is empty. camera_id="
                  << config.camera_id
                  << std::endl;
        return false;
    }

    if (!ensure_server_created_locked()) {
        return false;
    }

    const std::string mount_path = make_mount_path(config.camera_id);
    const std::string launch = make_h264_relay_launch(config.rtsp_uri);

    GstRTSPMountPoints* mounts = gst_rtsp_server_get_mount_points(server_);
    if (mounts == nullptr) {
        std::cerr << "RtspRelayServer: failed to get mount points" << std::endl;
        return false;
    }

    GstRTSPMediaFactory* factory = gst_rtsp_media_factory_new();
    if (factory == nullptr) {
        std::cerr << "RtspRelayServer: failed to create media factory" << std::endl;
        g_object_unref(mounts);
        return false;
    }

    // gst-rtsp-server의 launch 문자열에서 RTP payload element는 pay0 이름을 가져야 한다.
    gst_rtsp_media_factory_set_launch(factory, launch.c_str());

    // 여러 클라이언트가 같은 mount path에 붙을 때 같은 media pipeline을 공유하도록 설정한다.
    // 즉, 클라이언트마다 카메라에 직접 새 연결을 만들지 않도록 하기 위한 핵심 옵션이다.
    gst_rtsp_media_factory_set_shared(factory, TRUE);

    // mount point가 factory의 소유권을 가져간다.
    gst_rtsp_mount_points_add_factory(mounts, mount_path.c_str(), factory);
    g_object_unref(mounts);

    mounted_paths_.push_back(mount_path);

    std::cout << "RtspRelayServer: added H264 relay" << std::endl;
    std::cout << "  camera_id: " << config.camera_id << std::endl;
    std::cout << "  input    : " << config.rtsp_uri << std::endl;
    std::cout << "  output   : " << make_local_url(config.camera_id) << std::endl;
    std::cout << "  launch   : " << launch << std::endl;

    return true;
}

bool RtspRelayServer::start() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (running_) {
        return true;
    }

    if (!ensure_server_created_locked()) {
        return false;
    }

    main_loop_ = g_main_loop_new(nullptr, FALSE);
    if (main_loop_ == nullptr) {
        std::cerr << "RtspRelayServer: failed to create GMainLoop" << std::endl;
        return false;
    }

    // default GMainContext에 RTSP server source를 붙인다.
    // 이후 별도 thread에서 이 main loop를 실행한다.
    attach_id_ = gst_rtsp_server_attach(server_, nullptr);
    if (attach_id_ == 0) {
        std::cerr << "RtspRelayServer: failed to attach RTSP server. "
                  << "Check whether port " << service_ << " is already in use."
                  << std::endl;

        g_main_loop_unref(main_loop_);
        main_loop_ = nullptr;
        return false;
    }

    running_ = true;

    server_thread_ = std::thread([this]() {
        std::cout << "RtspRelayServer: main loop started on port "
                  << service_ << std::endl;
        g_main_loop_run(main_loop_);
        std::cout << "RtspRelayServer: main loop stopped" << std::endl;
    });

    std::cout << "RtspRelayServer started" << std::endl;
    for (const auto& path : mounted_paths_) {
        std::cout << "  rtsp://127.0.0.1:" << service_ << path << std::endl;
    }

    return true;
}

void RtspRelayServer::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!running_ && server_ == nullptr && main_loop_ == nullptr) {
            return;
        }

        if (main_loop_ != nullptr) {
            g_main_loop_quit(main_loop_);
        }
    }

    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    std::lock_guard<std::mutex> lock(mutex_);

    if (attach_id_ != 0) {
        g_source_remove(attach_id_);
        attach_id_ = 0;
    }

    if (main_loop_ != nullptr) {
        g_main_loop_unref(main_loop_);
        main_loop_ = nullptr;
    }

    if (server_ != nullptr) {
        g_object_unref(server_);
        server_ = nullptr;
    }

    mounted_paths_.clear();
    running_ = false;

    std::cout << "RtspRelayServer stopped" << std::endl;
}

std::string RtspRelayServer::make_mount_path(const std::string& camera_id) const {
    return "/live/" + camera_id;
}

std::string RtspRelayServer::make_local_url(const std::string& camera_id) const {
    return "rtsp://127.0.0.1:" + service_ + make_mount_path(camera_id);
}

bool RtspRelayServer::ensure_server_created_locked() {
    if (server_ != nullptr) {
        return true;
    }

    server_ = gst_rtsp_server_new();
    if (server_ == nullptr) {
        std::cerr << "RtspRelayServer: failed to create RTSP server" << std::endl;
        return false;
    }

    g_object_set(server_, "service", service_.c_str(), nullptr);
    return true;
}

std::string RtspRelayServer::make_h264_relay_launch(const std::string& rtsp_uri) const {
    // 26강 최소 relay 파이프라인.
    //
    // rtspsrc로 카메라 RTSP를 받고,
    // H264 RTP를 depay/parse한 뒤,
    // 다시 rtph264pay로 감싸 RTSP server가 클라이언트에게 내보낸다.
    //
    // decode/re-encode를 하지 않으므로 CPU 부하가 낮다.
    std::ostringstream oss;

    oss << "( "
        << "rtspsrc location=" << quote_for_gst_launch(rtsp_uri)
        << " latency=100 protocols=tcp "
        << "! rtph264depay "
        << "! h264parse config-interval=1 "
        << "! rtph264pay name=pay0 pt=96 "
        << ")";

    return oss.str();
}

std::string RtspRelayServer::quote_for_gst_launch(const std::string& value) const {
    std::string result;
    result.reserve(value.size() + 2);

    result.push_back('"');

    for (char ch : value) {
        if (ch == '\\' || ch == '"') {
            result.push_back('\\');
        }
        result.push_back(ch);
    }

    result.push_back('"');
    return result;
}
