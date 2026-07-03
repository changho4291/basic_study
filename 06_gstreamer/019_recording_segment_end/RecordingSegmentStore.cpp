#include "RecordingSegmentStore.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void RecordingSegmentStore::on_segment_started(
    const std::string& camera_id,
    const std::string& file_path
) {
    // 1. Segment 생성
    RecordingSegment segment;
    segment.camera_id = camera_id;
    segment.file_path = file_path;
    segment.started_at = std::chrono::system_clock::now();
    segment.status = "recording";

    // 2. 메모리 저장
    {
        std::lock_guard<std::mutex> lock(mutex_);
        segments_.push_back(segment);
    }

    // 3. 콘솔 출력
    std::cout << "[SEGMENT STARTED] "
              << "camera_id=" << camera_id
              << ", file=" << file_path
              << ", started_at=" << time_to_string(segment.started_at)
              << std::endl;
}

void RecordingSegmentStore::on_segment_finished(
    const std::string& camera_id,
    const std::string& file_path
) {
    auto ended_at = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(mutex_);

    // 1. 해당 Segment 찾기
    for (auto& segment : segments_) {
        if (segment.camera_id == camera_id &&
            segment.file_path == file_path &&
            segment.status == "recording") {
            segment.ended_at = ended_at;
            segment.status = "completed";

            // 2. 콘솔 출력
            std::cout << "[SEGMENT FINISHED] "
                      << "camera_id=" << camera_id
                      << ", file=" << file_path
                      << ", started_at=" << time_to_string(segment.started_at)
                      << ", ended_at=" << time_to_string(ended_at)
                      << std::endl;

            return;
        }
    }

    // 3. 이미 completed였거나 찾지 못한 경우
    std::cout << "[SEGMENT FINISHED] "
              << "camera_id=" << camera_id
              << ", file=" << file_path
              << ", warning=segment not found or already closed"
              << std::endl;
}

void RecordingSegmentStore::mark_open_segments_interrupted(
    const std::string& camera_id
) {
    auto ended_at = std::chrono::system_clock::now();

    std::lock_guard<std::mutex> lock(mutex_);

    // 1. 아직 recording 상태인 segment를 interrupted로 마킹
    for (auto& segment : segments_) {
        if (segment.camera_id == camera_id &&
            segment.status == "recording") {
            segment.ended_at = ended_at;
            segment.status = "interrupted";

            std::cout << "[SEGMENT INTERRUPTED] "
                      << "camera_id=" << camera_id
                      << ", file=" << segment.file_path
                      << ", started_at=" << time_to_string(segment.started_at)
                      << ", ended_at=" << time_to_string(ended_at)
                      << ", status=interrupted"
                      << std::endl;
        }
    }
}

void RecordingSegmentStore::print_all() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::cout << std::endl;
    std::cout << "==== Recording Segments ====" << std::endl;

    for (const auto& segment : segments_) {
        std::cout << "camera_id=" << segment.camera_id
                  << ", file=" << segment.file_path
                  << ", status=" << segment.status
                  << ", started_at=" << time_to_string(segment.started_at);

        if (segment.ended_at.has_value()) {
            std::cout << ", ended_at=" << time_to_string(*segment.ended_at);
        } else {
            std::cout << ", ended_at=null";
        }

        std::cout << std::endl;
    }

    std::cout << "==============================" << std::endl;
}

std::string RecordingSegmentStore::time_to_string(
    const std::chrono::system_clock::time_point& time 
) {
    std:time_t tt = std::chrono::system_clock::to_time_t(time);

    std::tm tm_value{};

#if defined(_WIN32)
    localtime_s(&tm_value, &tt);
#else
    localtime_r(&tt, &tm_value);
#endif

    std::ostringstream oss;

    oss << std::put_time(
        &tm_value,
        "%T-%m-%d %H:%M:%S"
    );

    return oss.str();
}