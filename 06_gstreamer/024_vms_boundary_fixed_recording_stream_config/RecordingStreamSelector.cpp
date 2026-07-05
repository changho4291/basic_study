#include "RecordingStreamSelector.h"

#include <algorithm>

namespace {

bool has_valid_resolution(const RecordingStreamConfig& config) {
    return config.width > 0 && config.height > 0;
}

int safe_bitrate(const RecordingStreamConfig& config) {
    return config.bitrate_kbps > 0 ? config.bitrate_kbps : 0;
}

int safe_fps(const RecordingStreamConfig& config) {
    return config.fps > 0 ? config.fps : 0;
}

} // namespace

std::optional<RecordingStreamConfig> RecordingStreamSelector::select_recording_stream(
    const std::vector<RecordingStreamConfig>& streams
) const {
    // 1. 녹화 가능한 stream만 후보로 모은다.
    std::vector<RecordingStreamConfig> candidates;

    for (const auto& config : streams) {
        if (!config.record_enabled) { continue; }
        if (config.rtsp_uri.empty()) { continue; }
        if (!is_supported_recording_codec(config.codec_hint)) { continue; }

        candidates.push_back(config);
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    // 2. 녹화용 선택 기준.
    //    - Main Stream 우선
    //    - H265/H264 같은 녹화 지원 코덱 우선
    //    - 해상도 높은 stream 우선
    //    - fps 높은 stream 우선
    //    - bitrate 높은 stream 우선
    std::sort(
        candidates.begin(),
        candidates.end(),
        [this](const RecordingStreamConfig& lhs, const RecordingStreamConfig& rhs) {
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

std::optional<RecordingStreamConfig> RecordingStreamSelector::select_live_view_stream(
    const std::vector<RecordingStreamConfig>& streams
) const {
    // 1. 라이브뷰 가능한 stream만 후보로 모은다.
    std::vector<RecordingStreamConfig> candidates;

    for (const auto& config : streams) {
        if (!config.live_view_enabled) { continue; }
        if (config.rtsp_uri.empty()) { continue; }

        candidates.push_back(config);
    }

    if (candidates.empty()) {
        return std::nullopt;
    }

    // 2. 라이브뷰 선택 기준.
    //    - Sub Stream 우선
    //    - 낮은 해상도 우선
    //    - 낮은 bitrate 우선
    //    - 낮은 fps 우선
    std::sort(
        candidates.begin(),
        candidates.end(),
        [this](const RecordingStreamConfig& lhs, const RecordingStreamConfig& rhs) {
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

std::optional<RecordingStreamConfig> RecordingStreamSelector::select_thumbnail_stream(
    const std::vector<RecordingStreamConfig>& streams
) const {
    // 1. 썸네일은 현재 라이브뷰와 같은 정책을 사용한다.
    //    나중에는 ONVIF Snapshot URI 또는 별도 snapshot 설정을 우선하도록 분리할 수 있다.
    return select_live_view_stream(streams);
}

bool RecordingStreamSelector::is_supported_recording_codec(
    StreamCodecHint codec_hint
) const {
    // 1. 현재 파이프라인은 H264 고정이지만,
    //    정책 레벨에서는 H265까지 후보로 열어둔다.
    //    실제 연결 가능 여부는 CameraPipeline의 runtime caps 검증에서 다시 확인한다.
    return codec_hint == StreamCodecHint::H264 ||
           codec_hint == StreamCodecHint::H265 ||
           codec_hint == StreamCodecHint::Unknown;
}

int RecordingStreamSelector::codec_recording_score(
    StreamCodecHint codec_hint
) const {
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

int RecordingStreamSelector::resolution_score(
    const RecordingStreamConfig& config
) const {
    if (!has_valid_resolution(config)) {
        return 0;
    }

    return config.width * config.height;
}
