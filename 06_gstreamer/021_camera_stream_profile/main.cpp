#include "CameraManager.h"

#include <gst/gst.h>

#include <chrono>
#include <csignal>
#include <iostream>

namespace {

volatile std::sig_atomic_t g_shutdown_requested = 0;

void handle_shutdown_signal(int signal_number) {
    // 1. signal handler에서는 종료 플래그만 설정
    if (signal_number == SIGINT || signal_number == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

} // namespace

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 종료 신호 핸들러 등록
    std::signal(SIGINT, handle_shutdown_signal);
    std::signal(SIGTERM, handle_shutdown_signal);

    // 3. CameraManager 생성
    CameraManager manager;

    // 4. VMS가 선택한 녹화용 스트림 프로파일 구성
    //    실제 VMS에서는 이 정보가 ONVIF 또는 자체 프로토콜 조회 결과에서 온다.
    CameraStreamProfile recording_profile;
    recording_profile.camera_id = "camera_001";
    recording_profile.profile_token = "profile_main_001";
    recording_profile.profile_name = "Main Stream";
    recording_profile.rtsp_uri = "rtsp://127.0.0.1:8554/test";

    recording_profile.codec_hint = StreamCodecHint::H264;
    recording_profile.width = 1920;
    recording_profile.height = 1080;
    recording_profile.fps = 30;
    recording_profile.bitrate_kbps = 4096;

    recording_profile.is_main_stream = true;
    recording_profile.is_sub_stream = false;
    recording_profile.record_enabled = true;
    recording_profile.live_view_enabled = true;

    recording_profile.record_pattern = "record_camera_001_main_%03d.mp4";
    recording_profile.split_seconds = 5;

    // 5. 카메라 등록
    manager.add_camera(recording_profile);

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 6. 전체 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 7. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 8. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;

    manager.stop_all();

    // 9. 전체 segment 출력
    manager.print_segments();

    // 10. 최근 10분 segment 조회
    auto query_to = std::chrono::system_clock::now();
    auto query_from = query_to - std::chrono::minutes(10);

    manager.print_segments_by_time_range(
        "camera_001",
        query_from,
        query_to
    );

    std::cout << "Program finished" << std::endl;

    return 0;
}