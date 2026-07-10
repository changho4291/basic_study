#include "CameraManager.h"
#include "RtspRelayServer.h"

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

CameraConfig make_lesson_camera_config() {
    // 실제 VMS에서는 이 값이 여기서 직접 만들어지지 않는다.
    //
    // 가능한 입력 경로:
    // - ONVIF provisioning 결과에서 선택된 profile의 GetStreamUri 결과
    // - DB에 저장된 카메라 녹화 설정
    // - 자체 프로토콜로 조회한 스트림 주소
    // - 사용자가 수동 등록한 RTSP 주소
    //
    // 중요한 점:
    // GStreamer 녹화 엔진과 RTSP relay 서버는 ONVIF profile token을 모르고,
    // 최종적으로 선택된 RTSP URI만 받아 동작한다.
    CameraConfig config;

    config.camera_id = "camera_001";
    config.rtsp_uri = "rtsp://127.0.0.1:8554/test";
    config.record_pattern = "record_camera_001_%03d.mp4";
    config.split_seconds = 5;

    return config;
}

} // namespace

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 종료 신호 핸들러 등록
    std::signal(SIGINT, handle_shutdown_signal);
    std::signal(SIGTERM, handle_shutdown_signal);

    // 3. 외부 계층에서 선택 완료된 카메라 설정 준비
    CameraConfig camera_config = make_lesson_camera_config();

    // 4. 녹화 엔진 생성 및 카메라 등록
    CameraManager manager;

    if (!manager.add_camera(camera_config)) {
        std::cerr << "Failed to add camera config" << std::endl;
        return 1;
    }

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 5. RTSP relay 서버 준비
    //    27강에서도 녹화 파이프라인과 relay 파이프라인은 아직 카메라 연결을 공유하지 않는다.
    //    즉, 녹화용 연결 1개 + relay용 연결 1개가 생길 수 있다.
    RtspRelayServer relay_server("8555");

    if (!relay_server.add_h264_camera(camera_config)) {
        std::cerr << "Failed to add camera to RTSP relay server" << std::endl;
        return 1;
    }

    if (!relay_server.start()) {
        std::cerr << "Failed to start RTSP relay server" << std::endl;
        return 1;
    }

    // 6. 녹화 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "VMS relay URL: "
              << relay_server.make_local_url(camera_config.camera_id)
              << std::endl;
    std::cout << "Test with one or more clients:" << std::endl;
    std::cout << "  ffplay "
              << relay_server.make_local_url(camera_config.camera_id)
              << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 7. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 8. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;

    std::cout << "Stopping RTSP relay server..." << std::endl;
    relay_server.stop();

    std::cout << "Stopping CameraManager..." << std::endl;
    manager.stop_all();

    // 9. 전체 segment 출력
    manager.print_segments();

    // 10. 최근 10분 segment 조회
    auto query_to = std::chrono::system_clock::now();
    auto query_from = query_to - std::chrono::minutes(10);

    manager.print_segments_by_time_range(
        camera_config.camera_id,
        query_from,
        query_to
    );

    std::cout << "Program finished" << std::endl;

    return 0;
}
