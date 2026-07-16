#pragma once

#include "HlsVodSession.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct HlsVodConvertResult {
    bool ok = false;
    std::string error_message;

    std::size_t sequence_no = 0;
    std::filesystem::path source_file_path;
    std::filesystem::path output_file_path;

    std::int64_t source_offset_ms = 0;
    std::int64_t requested_duration_ms = 0;

    // 36강에서는 아직 offset/duration trim을 적용하지 않는다.
    // 각 원본 MP4 파일 전체를 TS로 remux한다.
    bool trim_applied = false;
};

struct HlsVodConvertAllResult {
    bool ok = false;
    std::string error_message;

    std::string session_id;
    std::string camera_id;
    std::filesystem::path cache_dir;
    std::filesystem::path playlist_path;

    std::size_t requested_count = 0;
    std::size_t success_count = 0;
    std::size_t failure_count = 0;

    std::vector<HlsVodConvertResult> segment_results;
};

// MP4 녹화 segment를 HLS용 MPEG-TS segment로 변환하는 클래스다.
//
// 36강 범위:
// - HlsVodSession의 모든 segment plan을 순차 변환
// - MP4 전체를 TS로 remux
// - decode/re-encode 하지 않음
//
// 아직 하지 않는 것:
// - source_offset_ms seek 적용
// - duration_ms만큼 자르기
// - H264 이외 codec 자동 처리
class HlsVodConverter {
public:
    HlsVodConvertResult convert_first_segment(
        const HlsVodSession& session
    ) const;

    HlsVodConvertAllResult convert_all_segments(
        const HlsVodSession& session
    ) const;

    HlsVodConvertResult convert_segment_to_ts(
        const HlsVodSegment& segment
    ) const;

    static void print_convert_result(
        const HlsVodConvertResult& result
    );

    static void print_convert_all_result(
        const HlsVodConvertAllResult& result
    );

private:
    HlsVodConvertResult validate_segment(
        const HlsVodSegment& segment
    ) const;
};
