#pragma once

#include "CameraStreamProfile.h"

#include <optional>
#include <vector>

// VMS에서 여러 stream profile 중 용도에 맞는 profile을 선택하는 정책 클래스.
// 지금은 단순 규칙 기반이지만, 나중에는 DB 설정/사용자 정책/장치 성능에 따라 확장할 수 있다.
class CameraStreamProfileSelector {
public:
    // 녹화용 profile 선택.
    // 일반적으로 Main Stream, 높은 해상도, 지원 코덱을 우선한다.
    std::optional<CameraStreamProfile> select_recording_profile(
        const std::vector<CameraStreamProfile>& profiles
    ) const;

    // 라이브뷰용 profile 선택.
    // 일반적으로 Sub Stream, 낮은 bitrate, 낮은 해상도를 우선한다.
    std::optional<CameraStreamProfile> select_live_view_profile(
        const std::vector<CameraStreamProfile>& profiles
    ) const;

    // 썸네일/스냅샷 처리용 profile 선택.
    // 현재는 가장 가벼운 live view 가능 profile을 선택한다.
    std::optional<CameraStreamProfile> select_thumbnail_profile(
        const std::vector<CameraStreamProfile>& profiles
    ) const;

private:
    bool is_supported_recording_codec(StreamCodecHint codec_hint) const;
    int codec_recording_score(StreamCodecHint codec_hint) const;

    int resolution_score(const CameraStreamProfile& profile) const;
};
