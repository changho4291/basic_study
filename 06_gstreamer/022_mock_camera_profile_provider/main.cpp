#include "CameraManager.h"
#include "CameraStreamProfile.h"
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

std::optional<CameraStreamProfile> select_temporary_recording_profile(
    const std::vector<CameraStreamProfile>& profiles
) {
    // 1. 26강에서는 아직 별도 선택 정책 클래스를 만들지 않는다.
    //    일단 record_enabled=true 이면서 main stream인 profile을 우선 선택한다.
    for (const auto& profile : profiles) {
        if (profile.record_enabled && profile.is_main_stream) {
            return profile;
        }
    }

    // 2. main stream이 없으면 record_enabled=true인 첫 번째 profile을 선택한다.
    for (const auto& profile : profiles) {
        if (profile.record_enabled) {
            return profile;
        }
    }

    // 3. 녹화 가능한 profile이 없다면 실패
    return std::nullopt;
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

    // 5. 카메라의 stream profile 목록 조회
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

    // 6. 임시 녹화용 profile 선택
    //    본격적인 선택 정책은 다음 강의에서 별도 클래스로 분리한다.
    std::optional<CameraStreamProfile> selected_profile =
        select_temporary_recording_profile(profiles);

    if (!selected_profile.has_value()) {
        std::cerr << "No recordable stream profile found for camera_id="
                  << camera_id
                  << std::endl;
        return 1;
    }

    std::cout << "Selected recording profile: "
              << describe_camera_stream_profile(selected_profile.value())
              << std::endl;

    // 7. 선택된 profile을 CameraManager에 등록
    if (!manager.add_camera(selected_profile.value())) {
        std::cerr << "Failed to add selected camera stream profile" << std::endl;
        return 1;
    }

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 8. 전체 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 9. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 10. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;

    manager.stop_all();

    // 11. 전체 segment 출력
    manager.print_segments();

    // 12. 최근 10분 segment 조회
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
