#include "CameraManager.h"
#include "CameraStreamProfile.h"
#include "CameraStreamProfileSelector.h"
#include "MockCameraProfileProvider.h"

#include <gst/gst.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <optional>
#include <vector>

namespace {

volatile std::sig_atomic_t g_shutdown_requested = 0;

void handle_shutdown_signal(int signal_number) {
    // 1. signal handler에서는 종료 플래그만 설정
    if (signal_number == SIGINT || signal_number == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

void print_stream_profiles(
    const std::vector<CameraStreamProfile>& profiles
) {
    // 1. 조회된 profile 목록 출력
    std::cout << "===== Stream Profiles =====" << std::endl;

    for (const auto& profile : profiles) {
        std::cout << describe_camera_stream_profile(profile)
                  << std::endl;
    }

    std::cout << "===========================" << std::endl;
}

void print_selected_profile(
    const std::string& purpose,
    const std::optional<CameraStreamProfile>& profile
) {
    // 1. 선택 결과 출력
    if (!profile.has_value()) {
        std::cout << "Selected " << purpose << " profile: none"
                  << std::endl;
        return;
    }

    std::cout << "Selected " << purpose << " profile: "
              << describe_camera_stream_profile(profile.value())
              << std::endl;
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

    // 4. 프로파일 제공자 생성
    //    실제 VMS에서는 이 자리가 OnvifCameraProfileProvider 또는 자체 API Provider로 바뀐다.
    MockCameraProfileProvider profile_provider;

    // 5. 프로파일 선택 정책 객체 생성
    CameraStreamProfileSelector profile_selector;

    // 6. 카메라의 stream profile 목록 조회
    const std::string camera_id = "camera_001";

    std::vector<CameraStreamProfile> profiles =
        profile_provider.get_stream_profiles(camera_id);

    if (profiles.empty()) {
        std::cerr << "No stream profiles found for camera_id="
                  << camera_id
                  << std::endl;
        return 1;
    }

    print_stream_profiles(profiles);

    // 7. 용도별 profile 선택
    std::optional<CameraStreamProfile> recording_profile =
        profile_selector.select_recording_profile(profiles);

    std::optional<CameraStreamProfile> live_view_profile =
        profile_selector.select_live_view_profile(profiles);

    std::optional<CameraStreamProfile> thumbnail_profile =
        profile_selector.select_thumbnail_profile(profiles);

    print_selected_profile("recording", recording_profile);
    print_selected_profile("live view", live_view_profile);
    print_selected_profile("thumbnail", thumbnail_profile);

    if (!recording_profile.has_value()) {
        std::cerr << "No recordable stream profile found for camera_id="
                  << camera_id
                  << std::endl;
        return 1;
    }

    // 8. 이번 단계에서는 녹화용 profile만 CameraManager에 등록한다.
    //    live view / thumbnail profile은 다음 단계에서 별도 pipeline으로 분리할 수 있다.
    if (!manager.add_camera(recording_profile.value())) {
        std::cerr << "Failed to add selected recording stream profile" << std::endl;
        return 1;
    }

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 9. 전체 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 10. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 11. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;

    manager.stop_all();

    // 12. 전체 segment 출력
    manager.print_segments();

    // 13. 최근 10분 segment 조회
    auto query_to = std::chrono::system_clock::now();
    auto query_from = query_to - std::chrono::minutes(10);

    manager.print_segments_by_time_range(
        camera_id,
        query_from,
        query_to
    );

    std::cout << "Program finished" << std::endl;

    return 0;
}
