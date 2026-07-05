#pragma once

#include "RecordingStreamConfig.h"

#include <optional>
#include <vector>

// VMS 내부 녹화/라이브뷰 설정 목록 중 용도에 맞는 스트림을 선택하는 정책 클래스다.
// 이 클래스는 ONVIF를 모른다. 이미 변환된 RecordingStreamConfig만 다룬다.
class RecordingStreamSelector {
public:
    std::optional<RecordingStreamConfig> select_recording_stream(
        const std::vector<RecordingStreamConfig>& streams
    ) const;

    std::optional<RecordingStreamConfig> select_live_view_stream(
        const std::vector<RecordingStreamConfig>& streams
    ) const;

    std::optional<RecordingStreamConfig> select_thumbnail_stream(
        const std::vector<RecordingStreamConfig>& streams
    ) const;

private:
    bool is_supported_recording_codec(StreamCodecHint codec_hint) const;
    int codec_recording_score(StreamCodecHint codec_hint) const;
    int resolution_score(const RecordingStreamConfig& config) const;
};
