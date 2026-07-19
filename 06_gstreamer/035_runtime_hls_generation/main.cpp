#include "CameraManager.h"
#include "HlsVodSession.h"
#include "HlsVodPlaylistBuilder.h"
#include "HlsVodConverter.h"
#include "PlaybackService.h"

#include <gst/gst.h>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::vector<CameraConfig> make_lesson_camera_configs() {
    // 1. 예제 카메라 설정
    // 실제 VMS에서는 ONVIF/DB/자체 프로토콜 계층에서 이미 선택된 RTSP URI가 들어온다.
    std::vector<CameraConfig> configs;

    CameraConfig camera_001;
    camera_001.camera_id = "camera_001";
    camera_001.rtsp_uri = "rtsp://127.0.0.1:8554/test";
    camera_001.record_pattern = "record_camera_001_%03d.mp4";
    camera_001.split_seconds = 5;
    configs.push_back(camera_001);

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

void print_help() {
    std::cout << std::endl;
    std::cout << "===== Runtime Commands =====" << std::endl;
    std::cout << "help" << std::endl;
    std::cout << "  Print this help." << std::endl;
    std::cout << std::endl;
    std::cout << "segments" << std::endl;
    std::cout << "  Print all known recording segments." << std::endl;
    std::cout << std::endl;
    std::cout << "play <camera_id> last <seconds>" << std::endl;
    std::cout << "  Create an HLS VOD session while recording is still running." << std::endl;
    std::cout << "  Example: play camera_001 last 60" << std::endl;
    std::cout << std::endl;
    std::cout << "quit" << std::endl;
    std::cout << "  Stop recording and exit." << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << std::endl;
}

struct PlayLastCommand {
    std::string camera_id;
    int seconds = 0;
};

std::optional<PlayLastCommand> parse_play_last_command(const std::string& line) {
    std::istringstream iss(line);

    std::string play_token;
    std::string camera_id;
    std::string last_token;
    int seconds = 0;

    if (!(iss >> play_token >> camera_id >> last_token >> seconds)) {
        return std::nullopt;
    }

    std::string extra;
    if (iss >> extra) {
        return std::nullopt;
    }

    if (play_token != "play" || last_token != "last" || camera_id.empty() || seconds <= 0) {
        return std::nullopt;
    }

    PlayLastCommand command;
    command.camera_id = camera_id;
    command.seconds = seconds;
    return command;
}

bool create_hls_vod_session_for_last_seconds(
    const PlaybackService& playback_service,
    const HlsVodSessionManager& hls_session_manager,
    const std::string& camera_id,
    int seconds
) {
    // 1. 현재 실행 중인 RecordingSegmentStore에서 완료된 segment를 조회한다.
    // PlaybackService는 아직 recording 중인 segment를 HLS VOD 대상으로 쓰지 않는다.
    const auto query_to = std::chrono::system_clock::now();
    const auto query_from = query_to - std::chrono::seconds(seconds);

    PlaybackRequest request;
    request.camera_id = camera_id;
    request.from = query_from;
    request.to = query_to;

    std::cout << std::endl;
    std::cout << "===== Runtime HLS Playback Request =====" << std::endl;
    std::cout << "camera_id=" << camera_id << std::endl;
    std::cout << "range=last " << seconds << " seconds" << std::endl;
    std::cout << "Recording is still running while this request is processed." << std::endl;

    // 2. PlaybackSession 생성
    auto playback_response = playback_service.create_session(request);
    PlaybackService::print_response(playback_response);

    if (!playback_response.ok || !playback_response.session.has_value()) {
        std::cout << "Runtime playback request failed before HLS session creation." << std::endl;
        std::cout << "Hint: wait until at least one splitmuxsink segment is completed, then try again." << std::endl;
        return false;
    }

    // 3. HLS VOD session 디렉터리와 segment 계획 생성
    auto hls_result = hls_session_manager.create_session(*playback_response.session);
    HlsVodSessionManager::print_create_result(hls_result);

    if (!hls_result.ok || !hls_result.session.has_value()) {
        std::cout << "Runtime playback request failed during HLS session creation." << std::endl;
        return false;
    }

    const auto& hls_session = *hls_result.session;

    // 4. index.m3u8 생성
    HlsVodPlaylistBuilder playlist_builder;
    auto playlist_result = playlist_builder.write_playlist(hls_session);
    HlsVodPlaylistBuilder::print_write_result(playlist_result);

    if (!playlist_result.ok) {
        std::cout << "Runtime playback request failed while writing index.m3u8." << std::endl;
        return false;
    }

    // 5. TS segment 변환
    HlsVodConverter converter;
    auto convert_result = converter.convert_all_segments(hls_session);
    HlsVodConverter::print_convert_all_result(convert_result);

    if (!convert_result.ok) {
        std::cout << "Runtime playback request failed while converting MP4 segments to TS." << std::endl;
        return false;
    }

    // 6. HTTP 없이 로컬 파일 경로만 출력한다.
    std::cout << std::endl;
    std::cout << "===== Runtime HLS VOD Session Ready =====" << std::endl;
    std::cout << "session_id=" << hls_session.session_id << std::endl;
    std::cout << "playlist=" << hls_session.playlist_path.string() << std::endl;
    std::cout << "cache_dir=" << hls_session.cache_dir.string() << std::endl;
    std::cout << std::endl;
    std::cout << "Check files:" << std::endl;
    std::cout << "  cat " << hls_session.playlist_path.string() << std::endl;
    std::cout << "  ls -lh " << hls_session.cache_dir.string() << "/*.ts" << std::endl;
    std::cout << std::endl;
    std::cout << "Local playback test, depending on your environment:" << std::endl;
    std::cout << "  ffplay " << hls_session.playlist_path.string() << std::endl;
    std::cout << "========================================" << std::endl;

    return true;
}

} // namespace

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 카메라 설정 준비
    const std::vector<CameraConfig> camera_configs = make_lesson_camera_configs();
    if (camera_configs.empty()) {
        std::cerr << "No camera configs" << std::endl;
        return 1;
    }

    print_camera_configs(camera_configs);

    // 3. 녹화 엔진 준비
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

    std::cout << "Camera count: " << manager.camera_count() << std::endl;

    // 4. 녹화 시작
    // 이전 37강과 달리, 이 뒤에서 바로 종료 대기로 들어가지 않는다.
    // 녹화가 동작하는 동안 command loop에서 playback session을 생성한다.
    manager.start_all();
    std::cout << "CameraManager started" << std::endl;

    // 5. Playback/HLS 계층 준비
    PlaybackService playback_service(manager.segment_store());
    HlsVodSessionManager hls_session_manager("playback_cache");

    std::cout << std::endl;
    std::cout << "===== Lesson 38 Runtime HLS Generation =====" << std::endl;
    std::cout << "This lesson fixes the old flow where HLS was generated only after program shutdown." << std::endl;
    std::cout << "Recording keeps running. Type a playback command to generate HLS from completed segments." << std::endl;
    std::cout << "HLS playback cache root: " << hls_session_manager.cache_root().string() << std::endl;
    print_help();

    // 6. 실행 중 playback 요청 처리 loop
    bool running = true;
    while (running) {
        std::cout << "> " << std::flush;

        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) {
            continue;
        }

        if (line == "help") {
            print_help();
            continue;
        }

        if (line == "segments") {
            manager.print_segments();
            continue;
        }

        if (line == "quit" || line == "exit") {
            running = false;
            continue;
        }

        auto play_last = parse_play_last_command(line);
        if (play_last.has_value()) {
            create_hls_vod_session_for_last_seconds(
                playback_service,
                hls_session_manager,
                play_last->camera_id,
                play_last->seconds
            );
            continue;
        }

        std::cout << "Unknown command: " << line << std::endl;
        std::cout << "Type 'help' to see commands." << std::endl;
    }

    // 7. 종료 처리
    std::cout << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;
    manager.stop_all();
    manager.print_segments();

    std::cout << "Program finished" << std::endl;
    return 0;
}
