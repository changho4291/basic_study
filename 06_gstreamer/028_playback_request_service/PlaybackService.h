#pragma once

#include "RecordingSegmentStore.h"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct PlaybackRequest {
    std::string camera_id;
    std::chrono::system_clock::time_point from;
    std::chrono::system_clock::time_point to;
};

struct PlaybackEntry {
    std::string file_path;

    std::chrono::system_clock::time_point segment_started_at;
    std::chrono::system_clock::time_point segment_ended_at;

    std::chrono::system_clock::time_point play_from;
    std::chrono::system_clock::time_point play_to;

    std::int64_t play_from_offset_ms = 0;
    std::int64_t play_duration_ms = 0;

    std::string source_status;
};

struct PlaybackSession {
    std::string session_id;
    std::string camera_id;

    std::chrono::system_clock::time_point requested_from;
    std::chrono::system_clock::time_point requested_to;
    std::chrono::system_clock::time_point created_at;

    std::vector<PlaybackEntry> entries;
    std::int64_t total_duration_ms = 0;
};

struct PlaybackResponse {
    bool ok = false;
    std::string error_message;
    std::optional<PlaybackSession> session;
};

// 사용자의 과거 영상 재생 요청을 녹화 segment 목록으로 바꾸는 서비스다.
//
// 31강 범위:
// - HLS 파일 생성은 하지 않는다.
// - HTTP 응답도 하지 않는다.
// - 먼저 “어떤 파일의 어느 구간을 재생해야 하는지”만 확정한다.
class PlaybackService {
public:
    explicit PlaybackService(const RecordingSegmentStore& segment_store);

    PlaybackResponse create_session(const PlaybackRequest& request) const;

    static void print_response(const PlaybackResponse& response);

private:
    PlaybackResponse validate_request(const PlaybackRequest& request) const;

    static std::string make_session_id();

    static std::optional<PlaybackEntry> make_entry_from_segment(
        const RecordingSegment& segment,
        const PlaybackRequest& request
    );

    static std::string time_to_string(
        const std::chrono::system_clock::time_point& time
    );

private:
    const RecordingSegmentStore& segment_store_;
};
