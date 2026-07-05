#include "CameraStreamProfile.h"

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

std::string describe_camera_stream_profile(const CameraStreamProfile& profile) {
    std::ostringstream oss;

    oss << "camera_id=" << profile.camera_id
        << ", profile_token=" << profile.profile_token
        << ", profile_name=" << profile.profile_name
        << ", codec_hint=" << stream_codec_hint_to_string(profile.codec_hint)
        << ", resolution=" << profile.width << "x" << profile.height
        << ", fps=" << profile.fps
        << ", bitrate_kbps=" << profile.bitrate_kbps
        << ", main=" << (profile.is_main_stream ? "true" : "false")
        << ", sub=" << (profile.is_sub_stream ? "true" : "false")
        << ", record_enabled=" << (profile.record_enabled ? "true" : "false")
        << ", live_view_enabled=" << (profile.live_view_enabled ? "true" : "false")
        << ", split_seconds=" << profile.split_seconds
        << ", rtsp_uri=" << profile.rtsp_uri;

    return oss.str();
}
