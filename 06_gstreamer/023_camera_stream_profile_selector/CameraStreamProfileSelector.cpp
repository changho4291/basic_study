#include "CameraStreamProfileSelector.h"

#include <algorithm>

namespace {

bool has_valid_resolution(const CameraStreamProfile& profile) {
    return profile.width > 0 && profile.height > 0;
}

int safe_bitrate(const CameraStreamProfile& profile) {
    return profile.bitrate_kbps > 0 ? profile.bitrate_kbps : 0;
}

int safe_fps(const CameraStreamProfile& profile) {
    return profile.fps > 0 ? profile.fps : 0;
}

} // namespace

std::optional<CameraStreamProfile> CameraStreamProfileSelector::select_recording_profile(
    const std::vector<CameraStreamProfile>& profiles
) const {
    // 1. 녹화 가능한 profile만 후보로 모은다.
    std::vector<CameraStreamProfile> candidates;

    for (const auto& profile : profiles) {
        if (!profile.record_enabled) { continue; }
        if (profile.rtsp_uri.empty()) { continue; }
        if (!is_supported_recording_codec(profile.codec_hint)) { continue; }

        candidates.push_back(profile);
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    // 2. 녹화용 선택 기준.
    //    - Main Stream 우선
    //    - H265/H264 같은 녹화 지원 코덱 우선
    //    - 해상도 높은 profile 우선
    //    - fps 높은 profile 우선
    //    - bitrate 높은 profile 우선
    std::sort(
        candidates.begin(),
        candidates.end(),
        [this](const CameraStreamProfile& lhs, const CameraStreamProfile& rhs) {
            if (lhs.is_main_stream != rhs.is_main_stream) {
                return lhs.is_main_stream > rhs.is_main_stream;
            }

            const int lhs_codec_score = codec_recording_score(lhs.codec_hint);
            const int rhs_codec_score = codec_recording_score(rhs.codec_hint);
            if (lhs_codec_score != rhs_codec_score) {
                return lhs_codec_score > rhs_codec_score;
            }

            const int lhs_resolution = resolution_score(lhs);
            const int rhs_resolution = resolution_score(rhs);
            if (lhs_resolution != rhs_resolution) {
                return lhs_resolution > rhs_resolution;
            }

            const int lhs_fps = safe_fps(lhs);
            const int rhs_fps = safe_fps(rhs);
            if (lhs_fps != rhs_fps) {
                return lhs_fps > rhs_fps;
            }

            return safe_bitrate(lhs) > safe_bitrate(rhs);
        }
    );

    return candidates.front();
}

std::optional<CameraStreamProfile> CameraStreamProfileSelector::select_live_view_profile(
    const std::vector<CameraStreamProfile>& profiles
) const {
    // 1. 라이브뷰 가능한 profile만 후보로 모은다.
    std::vector<CameraStreamProfile> candidates;

    for (const auto& profile : profiles) {
        if (!profile.live_view_enabled) { continue; }
        if (profile.rtsp_uri.empty()) { continue; }

        candidates.push_back(profile);
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    // 2. 라이브뷰 선택 기준.
    //    - Sub Stream 우선
    //    - 낮은 해상도 우선
    //    - 낮은 bitrate 우선
    //    - fps는 너무 낮지 않게 하되, 현재는 낮은 쪽을 우선한다.
    std::sort(
        candidates.begin(),
        candidates.end(),
        [this](const CameraStreamProfile& lhs, const CameraStreamProfile& rhs) {
            if (lhs.is_sub_stream != rhs.is_sub_stream) {
                return lhs.is_sub_stream > rhs.is_sub_stream;
            }

            const int lhs_resolution = resolution_score(lhs);
            const int rhs_resolution = resolution_score(rhs);
            if (lhs_resolution != rhs_resolution) {
                return lhs_resolution < rhs_resolution;
            }

            const int lhs_bitrate = safe_bitrate(lhs);
            const int rhs_bitrate = safe_bitrate(rhs);
            if (lhs_bitrate != rhs_bitrate) {
                return lhs_bitrate < rhs_bitrate;
            }

            return safe_fps(lhs) < safe_fps(rhs);
        }
    );

    return candidates.front();
}

std::optional<CameraStreamProfile> CameraStreamProfileSelector::select_thumbnail_profile(
    const std::vector<CameraStreamProfile>& profiles
) const {
    // 1. 썸네일은 현재 라이브뷰와 같은 정책을 사용한다.
    //    나중에는 JPEG snapshot URI, ONVIF GetSnapshotUri 등을 우선하도록 분리할 수 있다.
    return select_live_view_profile(profiles);
}

bool CameraStreamProfileSelector::is_supported_recording_codec(
    StreamCodecHint codec_hint
) const {
    // 1. 현재 녹화 파이프라인은 H264 고정이지만,
    //    VMS 정책 레벨에서는 H265까지 지원 후보로 열어둔다.
    //    실제 H265 동적 연결은 이후 GStreamer 파이프라인 강의에서 추가한다.
    return codec_hint == StreamCodecHint::H264 ||
           codec_hint == StreamCodecHint::H265 ||
           codec_hint == StreamCodecHint::Unknown;
}

int CameraStreamProfileSelector::codec_recording_score(
    StreamCodecHint codec_hint
) const {
    // 1. 녹화 저장 효율 관점에서는 H265를 H264보다 높게 평가할 수 있다.
    //    단, 실제 GStreamer 지원 여부는 이후 Runtime caps 검증에서 다시 확인한다.
    switch (codec_hint) {
    case StreamCodecHint::H265:
        return 30;

    case StreamCodecHint::H264:
        return 20;

    case StreamCodecHint::Unknown:
        return 10;

    case StreamCodecHint::MJPEG:
    case StreamCodecHint::MPEG4:
    case StreamCodecHint::MPEG2:
    case StreamCodecHint::MPEG_TS:
    default:
        return 0;
    }
}

int CameraStreamProfileSelector::resolution_score(
    const CameraStreamProfile& profile
) const {
    if (!has_valid_resolution(profile)) {
        return 0;
    }

    return profile.width * profile.height;
}
