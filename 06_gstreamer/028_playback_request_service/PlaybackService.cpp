#include "PlaybackService.h"

#include <algorithm>
#include <atomic>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

std::int64_t milliseconds_between(
    const std::chrono::system_clock::time_point& from,
    const std::chrono::system_clock::time_point& to
) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        to - from
    ).count();
}

} // namespace

PlaybackService::PlaybackService(const RecordingSegmentStore& segment_store)
    : segment_store_(segment_store) {
}

PlaybackResponse PlaybackService::create_session(
    const PlaybackRequest& request
) const {
    // 1. 요청값 검증
    auto validation = validate_request(request);
    if (!validation.ok) {
        return validation;
    }

    // 2. 요청 시간 범위와 겹치는 녹화 segment 조회
    auto segments = segment_store_.find_segments_by_time_range(
        request.camera_id,
        request.from,
        request.to
    );

    // 3. 시간 순서 정렬
    std::sort(
        segments.begin(),
        segments.end(),
        [](const RecordingSegment& lhs, const RecordingSegment& rhs) {
            return lhs.started_at < rhs.started_at;
        }
    );

    // 4. PlaybackSession 생성
    PlaybackSession session;
    session.session_id = make_session_id();
    session.camera_id = request.camera_id;
    session.requested_from = request.from;
    session.requested_to = request.to;
    session.created_at = std::chrono::system_clock::now();

    // 5. 녹화 segment를 playback entry로 변환
    //    HLS VOD는 고정된 과거 파일을 대상으로 하므로, 아직 recording 중이고
    //    ended_at이 없는 segment는 이번 단계에서는 제외한다.
    for (const auto& segment : segments) {
        auto entry = make_entry_from_segment(
            segment,
            request
        );

        if (!entry.has_value()) {
            continue;
        }

        session.total_duration_ms += entry->play_duration_ms;
        session.entries.push_back(*entry);
    }

    // 6. 재생 가능한 segment가 없으면 실패 응답
    if (session.entries.empty()) {
        PlaybackResponse response;
        response.ok = false;
        response.error_message = "no playable recording segments for requested range";
        return response;
    }

    // 7. 성공 응답
    PlaybackResponse response;
    response.ok = true;
    response.session = session;
    return response;
}

void PlaybackService::print_response(const PlaybackResponse& response) {
    std::cout << std::endl;
    std::cout << "===== PlaybackService Response =====" << std::endl;

    if (!response.ok) {
        std::cout << "ok=false" << std::endl;
        std::cout << "error=" << response.error_message << std::endl;
        std::cout << "====================================" << std::endl;
        return;
    }

    if (!response.session.has_value()) {
        std::cout << "ok=false" << std::endl;
        std::cout << "error=session missing" << std::endl;
        std::cout << "====================================" << std::endl;
        return;
    }

    const auto& session = *response.session;

    std::cout << "ok=true" << std::endl;
    std::cout << "session_id=" << session.session_id << std::endl;
    std::cout << "camera_id=" << session.camera_id << std::endl;
    std::cout << "requested_from=" << time_to_string(session.requested_from) << std::endl;
    std::cout << "requested_to=" << time_to_string(session.requested_to) << std::endl;
    std::cout << "entry_count=" << session.entries.size() << std::endl;
    std::cout << "total_duration_ms=" << session.total_duration_ms << std::endl;
    std::cout << std::endl;

    for (std::size_t index = 0; index < session.entries.size(); ++index) {
        const auto& entry = session.entries[index];

        std::cout << "  [Entry " << index << "]" << std::endl;
        std::cout << "    file_path=" << entry.file_path << std::endl;
        std::cout << "    segment_started_at=" << time_to_string(entry.segment_started_at) << std::endl;
        std::cout << "    segment_ended_at=" << time_to_string(entry.segment_ended_at) << std::endl;
        std::cout << "    play_from=" << time_to_string(entry.play_from) << std::endl;
        std::cout << "    play_to=" << time_to_string(entry.play_to) << std::endl;
        std::cout << "    play_from_offset_ms=" << entry.play_from_offset_ms << std::endl;
        std::cout << "    play_duration_ms=" << entry.play_duration_ms << std::endl;
        std::cout << "    source_status=" << entry.source_status << std::endl;
    }

    std::cout << "====================================" << std::endl;
}

PlaybackResponse PlaybackService::validate_request(
    const PlaybackRequest& request
) const {
    PlaybackResponse response;

    if (request.camera_id.empty()) {
        response.ok = false;
        response.error_message = "camera_id is empty";
        return response;
    }

    if (request.from >= request.to) {
        response.ok = false;
        response.error_message = "invalid playback time range";
        return response;
    }

    response.ok = true;
    return response;
}

std::string PlaybackService::make_session_id() {
    static std::atomic<std::uint64_t> counter{0};

    const auto now = std::chrono::system_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    std::ostringstream oss;
    oss << "session_" << millis << "_" << counter.fetch_add(1);

    return oss.str();
}

std::optional<PlaybackEntry> PlaybackService::make_entry_from_segment(
    const RecordingSegment& segment,
    const PlaybackRequest& request
) {
    // 1. HLS VOD는 완료된 파일을 대상으로 한다.
    //    recording 상태처럼 아직 파일이 닫히지 않은 segment는 이번 단계에서 제외한다.
    if (!segment.ended_at.has_value()) {
        return std::nullopt;
    }

    if (segment.status != "completed" &&
        segment.status != "interrupted") {
        return std::nullopt;
    }

    const auto segment_end = *segment.ended_at;

    // 2. 요청 범위와 실제 segment 범위의 교집합을 구한다.
    const auto play_from = std::max(
        segment.started_at,
        request.from
    );

    const auto play_to = std::min(
        segment_end,
        request.to
    );

    if (play_from >= play_to) {
        return std::nullopt;
    }

    // 3. 파일 내부 offset과 실제 재생 길이를 계산한다.
    PlaybackEntry entry;
    entry.file_path = segment.file_path;
    entry.segment_started_at = segment.started_at;
    entry.segment_ended_at = segment_end;
    entry.play_from = play_from;
    entry.play_to = play_to;
    entry.play_from_offset_ms = milliseconds_between(
        segment.started_at,
        play_from
    );
    entry.play_duration_ms = milliseconds_between(
        play_from,
        play_to
    );
    entry.source_status = segment.status;

    if (entry.play_duration_ms <= 0) {
        return std::nullopt;
    }

    return entry;
}

std::string PlaybackService::time_to_string(
    const std::chrono::system_clock::time_point& time
) {
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        time.time_since_epoch()
    ).count() % 1000;

    std::time_t tt = std::chrono::system_clock::to_time_t(time);

    std::tm tm_value{};

#if defined(_WIN32)
    localtime_s(&tm_value, &tt);
#else
    localtime_r(&tt, &tm_value);
#endif

    std::ostringstream oss;
    oss << std::put_time(
        &tm_value,
        "%Y-%m-%d %H:%M:%S"
    );
    oss << "." << std::setw(3) << std::setfill('0') << millis;

    return oss.str();
}
