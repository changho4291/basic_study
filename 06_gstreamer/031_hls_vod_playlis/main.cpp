#include "CameraManager.h"
#include "HlsVodSession.h"
#include "HlsVodPlaylistBuilder.h"
#include "PlaybackService.h"
#include "RtspRelayServer.h"

#include <gst/gst.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <vector>

namespace {

volatile std::sig_atomic_t g_shutdown_requested = 0;

void handle_shutdown_signal(int signal_number) {
    // 1. signal handler에서는 종료 플래그만 설정
    if (signal_number == SIGINT || signal_number == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

std::vector<CameraConfig> make_lesson_camera_configs() {
    // 실제 VMS에서는 이 목록이 main.cpp에서 직접 만들어지지 않는다.
    //
    // 가능한 입력 경로:
    // - ONVIF provisioning + DB에서 녹화 대상으로 선택된 카메라 목록
    // - 자체 프로토콜로 조회한 카메라 stream 목록
    // - 사용자가 수동 등록한 RTSP 주소 목록
    //
    // 이번 강의에서는 현재 구조의 연결 수와 생명주기를 확인하기 위해
    // CameraConfig 목록을 직접 만든다.
    std::vector<CameraConfig> configs;

    CameraConfig camera_001;
    camera_001.camera_id = "camera_001";
    camera_001.rtsp_uri = "rtsp://127.0.0.1:8554/test";
    camera_001.record_pattern = "record_camera_001_%03d.mp4";
    camera_001.split_seconds = 5;
    configs.push_back(camera_001);

    CameraConfig camera_002;
    camera_002.camera_id = "camera_002";
    camera_002.rtsp_uri = "rtsp://127.0.0.1:8554/test";
    camera_002.record_pattern = "record_camera_002_%03d.mp4";
    camera_002.split_seconds = 5;
    configs.push_back(camera_002);

    return configs;
}

void print_camera_configs(const std::vector<CameraConfig>& configs) {
    std::cout << "===== Camera Configs =====" << std::endl;

    for (const auto& config : configs) {
        std::cout << "camera_id=" << config.camera_id
                  << ", rtsp_uri=" << config.rtsp_uri
                  << ", record_pattern=" << config.record_pattern
                  << ", split_seconds=" << config.split_seconds
                  << std::endl;
    }

    std::cout << "==========================" << std::endl;
}

void print_pipeline_topology(
    const std::vector<CameraConfig>& configs,
    const RtspRelayServer& relay_server
) {
    std::cout << std::endl;
    std::cout << "===== VMS Pipeline Topology =====" << std::endl;
    std::cout << "This lesson verifies the current separated pipeline structure." << std::endl;
    std::cout << "Recording and RTSP relay do NOT share the same upstream pipeline yet." << std::endl;
    std::cout << std::endl;

    for (const auto& config : configs) {
        std::cout << "camera_id=" << config.camera_id << std::endl;
        std::cout << std::endl;

        std::cout << "  [Recording Pipeline]" << std::endl;
        std::cout << "    owner    : CameraManager / CameraPipeline" << std::endl;
        std::cout << "    upstream : " << config.rtsp_uri << std::endl;
        std::cout << "    purpose  : split recording + frame extraction" << std::endl;
        std::cout << "    trigger  : starts when CameraManager.start_all() is called" << std::endl;
        std::cout << "    client   : independent from live-view clients" << std::endl;
        std::cout << std::endl;

        std::cout << "  [Relay Pipeline]" << std::endl;
        std::cout << "    owner    : RtspRelayServer / gst-rtsp-server" << std::endl;
        std::cout << "    upstream : " << config.rtsp_uri << std::endl;
        std::cout << "    output   : " << relay_server.make_local_url(config.camera_id) << std::endl;
        std::cout << "    purpose  : live fan-out to many RTSP clients" << std::endl;
        std::cout << "    trigger  : media is prepared when a client connects" << std::endl;
        std::cout << "    shared   : true for clients using the same mount path" << std::endl;
        std::cout << std::endl;

        std::cout << "  [Important]" << std::endl;
        std::cout << "    current upstream connection model:" << std::endl;
        std::cout << "      1 connection for recording pipeline" << std::endl;
        std::cout << "      1 connection for relay pipeline, shared by live clients" << std::endl;
        std::cout << "    so this camera may have up to 2 RTSP upstream connections." << std::endl;
        std::cout << std::endl;
    }

    std::cout << "=================================" << std::endl;
    std::cout << std::endl;
}

void print_test_scenarios(
    const std::vector<CameraConfig>& configs,
    const RtspRelayServer& relay_server
) {
    std::cout << "===== Lesson 29 Test Scenarios =====" << std::endl;
    std::cout << "Scenario 1: Run this program without any live-view client." << std::endl;
    std::cout << "  Expected: recording starts and segments are created." << std::endl;
    std::cout << "  Expected: relay media-configure/prepared may not appear yet." << std::endl;
    std::cout << std::endl;

    std::cout << "Scenario 2: Open one RTSP client." << std::endl;
    if (!configs.empty()) {
        std::cout << "  ffplay " << relay_server.make_local_url(configs.front().camera_id) << std::endl;
    }
    std::cout << "  Expected: RtspRelayServer media-configure/prepared logs appear." << std::endl;
    std::cout << std::endl;

    std::cout << "Scenario 3: Open multiple clients for the same URL." << std::endl;
    if (!configs.empty()) {
        std::cout << "  ffplay " << relay_server.make_local_url(configs.front().camera_id) << std::endl;
        std::cout << "  ffplay " << relay_server.make_local_url(configs.front().camera_id) << std::endl;
        std::cout << "  ffplay " << relay_server.make_local_url(configs.front().camera_id) << std::endl;
    }
    std::cout << "  Expected: clients share the relay media for the same mount path." << std::endl;
    std::cout << std::endl;

    std::cout << "Scenario 4: Close every live-view client." << std::endl;
    std::cout << "  Expected: relay media unprepared may appear." << std::endl;
    std::cout << "  Expected: recording continues until this program is stopped." << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;
}

} // namespace

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 종료 신호 핸들러 등록
    std::signal(SIGINT, handle_shutdown_signal);
    std::signal(SIGTERM, handle_shutdown_signal);

    // 3. 외부 계층에서 선택 완료된 카메라 설정 목록 준비
    const std::vector<CameraConfig> camera_configs = make_lesson_camera_configs();

    if (camera_configs.empty()) {
        std::cerr << "No camera configs" << std::endl;
        return 1;
    }

    print_camera_configs(camera_configs);

    // 4. 녹화 엔진 생성 및 여러 카메라 등록
    //    이쪽은 녹화용 CameraPipeline을 만들며, 클라이언트 접속 여부와 무관하게 동작한다.
    CameraManager manager;

    std::cout << "[Setup] Register recording pipelines" << std::endl;
    for (const auto& config : camera_configs) {
        std::cout << "[Recording Setup] camera_id=" << config.camera_id
                  << " upstream=" << config.rtsp_uri
                  << std::endl;

        if (!manager.add_camera(config)) {
            std::cerr << "Failed to add camera config. camera_id="
                      << config.camera_id << std::endl;
            return 1;
        }
    }

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 5. RTSP relay 서버 준비 및 여러 mount point 등록
    //    이쪽은 live-view client에게 RTSP를 재송출한다.
    //    녹화용 CameraPipeline과 upstream rtspsrc를 공유하지 않는다.
    RtspRelayServer relay_server("8555");

    std::cout << "[Setup] Register RTSP relay mounts" << std::endl;
    if (!relay_server.add_h264_cameras(camera_configs)) {
        std::cerr << "Failed to add one or more cameras to RTSP relay server" << std::endl;
        return 1;
    }

    print_pipeline_topology(camera_configs, relay_server);

    // 6. RTSP relay server 시작
    //    서버 자체는 여기서 시작하지만, 각 mount의 media pipeline은 client 접속 시 준비될 수 있다.
    if (!relay_server.start()) {
        std::cerr << "Failed to start RTSP relay server" << std::endl;
        return 1;
    }

    // 7. 녹화 시작
    //    녹화는 live-view client와 무관하게 시작된다.
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    relay_server.print_mounted_urls();
    print_test_scenarios(camera_configs, relay_server);

    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 8. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 9. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;

    // relay를 먼저 멈추면 live-view client 쪽 media가 정리된다.
    std::cout << "Stopping RTSP relay server..." << std::endl;
    relay_server.stop();

    // 녹화 파이프라인은 별도로 정리한다.
    std::cout << "Stopping CameraManager..." << std::endl;
    manager.stop_all();

    // 10. 전체 segment 출력
    manager.print_segments();

    // 11. 최근 10분 segment 조회
    auto query_to = std::chrono::system_clock::now();
    auto query_from = query_to - std::chrono::minutes(10);

    for (const auto& config : camera_configs) {
        manager.print_segments_by_time_range(
            config.camera_id,
            query_from,
            query_to
        );
    }

    // 12. PlaybackService 테스트
    //     아직 HLS 파일을 생성하지 않는다.
    //     먼저 사용자의 시간 범위 요청이 어떤 playback entry 목록으로 바뀌는지 확인한다.
    PlaybackService playback_service(manager.segment_store());

    std::cout << std::endl;
    std::cout << "===== Lesson 34 HLS VOD Playlist Test =====" << std::endl;
    std::cout << "Create playback sessions for the last 10 minutes, create HLS VOD session directories, then write index.m3u8." << std::endl;
    std::cout << "This step creates HLS session metadata, cache directories, and index.m3u8. TS files are not generated yet." << std::endl;

    HlsVodSessionManager hls_session_manager("playback_cache");
    std::cout << "HLS playback cache root: "
              << hls_session_manager.cache_root().string()
              << std::endl;

    for (const auto& config : camera_configs) {
        PlaybackRequest request;
        request.camera_id = config.camera_id;
        request.from = query_from;
        request.to = query_to;

        auto response = playback_service.create_session(request);
        PlaybackService::print_response(response);

        if (!response.ok || !response.session.has_value()) {
            continue;
        }

        const auto& session = *response.session;
        if (session.entries.empty()) {
            continue;
        }

        // 13. HLS VOD session metadata 생성
        //     playback_cache/session_xxx/ 디렉터리와 segment 계획을 만든다.
        auto hls_result = hls_session_manager.create_session(session);
        HlsVodSessionManager::print_create_result(hls_result);

        // 14. HLS VOD playlist 생성
        //     이번 단계에서는 index.m3u8만 작성한다.
        //     seg_000.ts 같은 실제 media segment 파일은 35강부터 생성한다.
        if (hls_result.ok && hls_result.session.has_value()) {
            HlsVodPlaylistBuilder playlist_builder;
            auto playlist_result = playlist_builder.write_playlist(*hls_result.session);
            HlsVodPlaylistBuilder::print_write_result(playlist_result);
        }

        // 15. Playback seek 테스트
        //     첫 번째 entry의 중간 지점으로 seek한다고 가정한다.
        //     실제 API에서는 사용자가 타임라인을 클릭한 절대 시각이 seek_time이 된다.
        const auto& first_entry = session.entries.front();
        const auto half_duration = std::chrono::milliseconds(
            std::max<std::int64_t>(1, first_entry.play_duration_ms / 2)
        );

        PlaybackSeekRequest seek_request;
        seek_request.session_id = session.session_id;
        seek_request.seek_time = first_entry.play_from + half_duration;

        if (seek_request.seek_time >= first_entry.play_to) {
            seek_request.seek_time = first_entry.play_from;
        }

        auto seek_result = playback_service.seek(
            session,
            seek_request
        );

        PlaybackService::print_seek_result(seek_result);
    }

    std::cout << "Program finished" << std::endl;

    return 0;
}
