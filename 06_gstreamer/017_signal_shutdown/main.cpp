#include "CameraManager.h"

#include <gst/gst.h>

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

    // 4. 카메라 등록
    manager.add_camera({
        "camera_001",
        "rtsp://127.0.0.1:8554/test",
        "record_camera_001_%03d.mp4",
        5
    });

    manager.add_camera({
        "camera_002",
        "rtsp://127.0.0.1:8554/test",
        "record_camera_002_%03d.mp4",
        5
    });

    // 5. 모든 카메라 시작
    manager.start_all();

    std::cout << "CameraManater started" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 6. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 7. 안전 종료
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;

    manager.stop_all();

    std::cout << "Program finished" << std::endl;

    return 0;
}
