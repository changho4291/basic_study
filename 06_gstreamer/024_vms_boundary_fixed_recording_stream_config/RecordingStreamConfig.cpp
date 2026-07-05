#include "RecordingStreamConfig.h"

#include <sstream>

std::string stream_codec_hint_to_string(StreamCodecHint codec_hint) {
    switch (codec_hint) {
    case StreamCodecHint::H264:
        return "H264";

    case StreamCodecHint::H265:
        return "H265";

    case StreamCodecHint::MJPEG:
        return "MJPEG";

    case StreamCodecHint::MPEG4:
        return "MPEG4";

    case StreamCodecHint::MPEG2:
        return "MPEG2";

    case StreamCodecHint::MPEG_TS:
        return "MPEG_TS";

    case StreamCodecHint::Unknown:
    default:
        return "Unknown";
    }
}

std::string describe_recording_stream_config(const RecordingStreamConfig& config) {
    std::ostringstream oss;

    oss << "camera_id=" << config.camera_id
        << ", stream_id=" << config.stream_id
        << ", stream_name=" << config.stream_name
        << ", source_type=" << config.source_ref.source_type
        << ", external_device_id=" << config.source_ref.external_device_id
        << ", external_stream_id=" << config.source_ref.external_stream_id
        << ", codec_hint=" << stream_codec_hint_to_string(config.codec_hint)
        << ", resolution=" << config.width << "x" << config.height
        << ", fps=" << config.fps
        << ", bitrate_kbps=" << config.bitrate_kbps
        << ", main=" << (config.is_main_stream ? "true" : "false")
        << ", sub=" << (config.is_sub_stream ? "true" : "false")
        << ", record_enabled=" << (config.record_enabled ? "true" : "false")
        << ", live_view_enabled=" << (config.live_view_enabled ? "true" : "false")
        << ", split_seconds=" << config.split_seconds
        << ", rtsp_uri=" << config.rtsp_uri;

    return oss.str();
}
