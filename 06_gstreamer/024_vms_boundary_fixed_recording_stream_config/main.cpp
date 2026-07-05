#include "CameraManager.h"
#include "RecordingStreamConfig.h"
#include "RecordingStreamSelector.h"

#include <gst/gst.h>
#include <glib.h>

#include <chrono>
#include <csignal>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {

volatile std::sig_atomic_t g_shutdown_requested = 0;

void handle_shutdown_signal(int signal_number) {
    // 1. signal handler에서는 종료 플래그만 설정
    if (signal_number == SIGINT || signal_number == SIGTERM) {
        g_shutdown_requested = 1;
    }
}

std::vector<RecordingStreamConfig> make_lesson_recording_stream_configs() {
    // 1. 이번 수정본에서는 ONVIF Provider를 새로 만들지 않는다.
    //    실제 프로젝트에서는 이미 존재하는 ProvisioningResult를 mapper로 변환해
    //    RecordingStreamConfig 목록을 만들면 된다.
    std::vector<RecordingStreamConfig> configs;

    RecordingStreamConfig main_stream;
    main_stream.camera_id = "camera_001";
    main_stream.stream_id = "recording_main";
    main_stream.stream_name = "Main Stream";
    main_stream.rtsp_uri = "rtsp://127.0.0.1:8554/test";

    // 2. 외부 출처 추적용 메타데이터.
    //    GStreamer는 이 값을 사용하지 않는다.
    main_stream.source_ref.source_type = "manual";
    main_stream.source_ref.external_device_id = "camera_001";
    main_stream.source_ref.external_stream_id = "profile_main_001";

    main_stream.codec_hint = StreamCodecHint::H264;
    main_stream.width = 1920;
    main_stream.height = 1080;
    main_stream.fps = 30;
    main_stream.bitrate_kbps = 4096;

    main_stream.is_main_stream = true;
    main_stream.is_sub_stream = false;
    main_stream.record_enabled = true;
    main_stream.live_view_enabled = true;

    main_stream.record_pattern = "record_camera_001_main_%03d.mp4";
    main_stream.split_seconds = 5;

    configs.push_back(main_stream);

    RecordingStreamConfig sub_stream;
    sub_stream.camera_id = "camera_001";
    sub_stream.stream_id = "live_sub";
    sub_stream.stream_name = "Sub Stream";
    sub_stream.rtsp_uri = "rtsp://127.0.0.1:8554/test_sub";

    // 3. 이것도 추적용 메타데이터일 뿐이다.
    sub_stream.source_ref.source_type = "manual";
    sub_stream.source_ref.external_device_id = "camera_001";
    sub_stream.source_ref.external_stream_id = "profile_sub_001";

    sub_stream.codec_hint = StreamCodecHint::H264;
    sub_stream.width = 640;
    sub_stream.height = 360;
    sub_stream.fps = 15;
    sub_stream.bitrate_kbps = 512;

    sub_stream.is_main_stream = false;
    sub_stream.is_sub_stream = true;
    sub_stream.record_enabled = false;
    sub_stream.live_view_enabled = true;

    sub_stream.record_pattern = "record_camera_001_sub_%03d.mp4";
    sub_stream.split_seconds = 5;

    configs.push_back(sub_stream);

    return configs;
}

void print_recording_stream_configs(
    const std::vector<RecordingStreamConfig>& configs
) {
    std::cout << "===== Recording Stream Configs =====" << std::endl;

    for (const auto& config : configs) {
        std::cout << describe_recording_stream_config(config)
                  << std::endl;
    }

    std::cout << "====================================" << std::endl;
}

void print_selected_stream(
    const std::string& purpose,
    const std::optional<RecordingStreamConfig>& config
) {
    if (!config.has_value()) {
        std::cout << "Selected " << purpose << " stream: none"
                  << std::endl;
        return;
    }

    std::cout << "Selected " << purpose << " stream: "
              << describe_recording_stream_config(config.value())
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

    // 4. 녹화 엔진이 사용할 내부 설정 목록 생성
    //    실제 시스템에서는 ONVIF provisioning 모듈 또는 DB 동기화 계층이
    //    이 RecordingStreamConfig 목록을 만들어 넘겨준다.
    const std::vector<RecordingStreamConfig> stream_configs =
        make_lesson_recording_stream_configs();

    if (stream_configs.empty()) {
        std::cerr << "No recording stream config found" << std::endl;
        return 1;
    }

    print_recording_stream_configs(stream_configs);

    // 5. 용도별 스트림 선택
    RecordingStreamSelector selector;

    std::optional<RecordingStreamConfig> recording_stream =
        selector.select_recording_stream(stream_configs);

    std::optional<RecordingStreamConfig> live_view_stream =
        selector.select_live_view_stream(stream_configs);

    std::optional<RecordingStreamConfig> thumbnail_stream =
        selector.select_thumbnail_stream(stream_configs);

    print_selected_stream("recording", recording_stream);
    print_selected_stream("live view", live_view_stream);
    print_selected_stream("thumbnail", thumbnail_stream);

    if (!recording_stream.has_value()) {
        std::cerr << "No recordable stream config found" << std::endl;
        return 1;
    }

    // 6. 이번 단계에서는 녹화용 stream만 CameraManager에 등록한다.
    if (!manager.add_camera(recording_stream.value())) {
        std::cerr << "Failed to add selected recording stream" << std::endl;
        return 1;
    }

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 7. 전체 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // 8. 종료 신호 대기
    while (g_shutdown_requested == 0) {
        g_usleep(100 * 1000);
    }

    // 9. 종료 처리
    std::cout << std::endl;
    std::cout << "Shutdown signal received" << std::endl;
    std::cout << "Stopping CameraManager..." << std::endl;

    manager.stop_all();

    // 10. 전체 segment 출력
    manager.print_segments();

    // 11. 최근 10분 segment 조회
    auto query_to = std::chrono::system_clock::now();
    auto query_from = query_to - std::chrono::minutes(10);

    manager.print_segments_by_time_range(
        recording_stream->camera_id,
        query_from,
        query_to
    );

    std::cout << "Program finished" << std::endl;

    return 0;
}
