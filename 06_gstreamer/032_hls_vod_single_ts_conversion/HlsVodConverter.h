#pragma once

#include "HlsVodSession.h"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

struct HlsVodConvertResult {
    bool ok = false;
    std::string error_message;

    std::size_t sequence_no = 0;
    std::filesystem::path source_file_path;
    std::filesystem::path output_file_path;

    std::int64_t source_offset_ms = 0;
    std::int64_t requested_duration_ms = 0;

    // 35강에서는 아직 offset/duration trim을 적용하지 않는다.
    // 단일 MP4 파일 전체를 TS로 remux하는 PoC만 수행한다.
    bool trim_applied = false;
};

// MP4 녹화 segment를 HLS용 MPEG-TS segment로 변환하는 클래스다.
//
// 35강 범위:
// - HlsVodSession의 첫 번째 segment만 변환
// - MP4 전체를 TS로 remux
// - decode/re-encode 하지 않음
//
// 아직 하지 않는 것:
// - 여러 segment 변환
// - source_offset_ms seek 적용
// - duration_ms만큼 자르기
// - H264 이외 codec 자동 처리
class HlsVodConverter {
public:
    HlsVodConvertResult convert_first_segment(
        const HlsVodSession& session
    ) const;

    HlsVodConvertResult convert_segment_to_ts(
        const HlsVodSegment& segment
    ) const;

    static void print_convert_result(
        const HlsVodConvertResult& result
    );

private:
    HlsVodConvertResult validate_segment(
        const HlsVodSegment& segment
    ) const;
};
