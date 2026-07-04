#include "VideoStreamProfile.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace {

std::string trim_copy(const std::string& value) {
    const auto begin = std::find_if(
        value.begin(),
        value.end(),
        [](unsigned char ch) {
            return !std::isspace(ch);
        }
    );

    if (begin == value.end()) {
        return "";
    }

    const auto end = std::find_if(
        value.rbegin(),
        value.rend(),
        [](unsigned char ch) {
            return !std::isspace(ch);
        }
    ).base();

    return std::string(begin, end);
}

std::string to_upper_copy(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char ch) {
            return static_cast<char>(std::toupper(ch));
        }
    );

    return value;
}

VideoStreamProfile make_unknown_profile(const std::string& encoding_name) {
    VideoStreamProfile profile;

    profile.codec = VideoCodec::Unknown;
    profile.name = "Unknown";
    profile.encoding_name = encoding_name;

    profile.depay_element = "";
    profile.parser_element = "";
    profile.decoder_element = "";

    profile.supported = false;
    profile.record_to_mp4_supported = false;
    profile.appsink_supported = false;

    return profile;
}

} // namespace

VideoStreamProfile make_video_stream_profile_from_encoding_name(
    const std::string& encoding_name
) {
    const std::string original = trim_copy(encoding_name);
    const std::string normalized = to_upper_copy(original);

    if (normalized.empty()) {
        return make_unknown_profile(original);
    }

    // 1. H.264 RTP
    if (normalized == "H264") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::H264;
        profile.name = "H264";
        profile.encoding_name = original;

        profile.depay_element = "rtph264depay";
        profile.parser_element = "h264parse";
        profile.decoder_element = "avdec_h264";

        profile.supported = true;
        profile.record_to_mp4_supported = true;
        profile.appsink_supported = true;

        return profile;
    }

    // 2. H.265 RTP
    // 일부 장비나 서버는 H265 대신 HEVC로 표시할 수 있다.
    if (normalized == "H265" || normalized == "HEVC") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::H265;
        profile.name = "H265";
        profile.encoding_name = original;

        profile.depay_element = "rtph265depay";
        profile.parser_element = "h265parse";
        profile.decoder_element = "avdec_h265";

        profile.supported = true;
        profile.record_to_mp4_supported = true;
        profile.appsink_supported = true;

        return profile;
    }

    // 3. MJPEG RTP
    // 감지는 하지만 26강 시점에서는 VMS 녹화 파이프라인 지원 대상에서 제외한다.
    if (normalized == "JPEG" || normalized == "MJPEG") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::MJPEG;
        profile.name = "MJPEG";
        profile.encoding_name = original;

        profile.depay_element = "rtpjpegdepay";
        profile.parser_element = "";
        profile.decoder_element = "jpegdec";

        profile.supported = false;
        profile.record_to_mp4_supported = false;
        profile.appsink_supported = false;

        return profile;
    }

    // 4. MPEG-4 Part 2 Video RTP
    // MP4V-ES는 MP4 파일 컨테이너가 아니라 RTP payload encoding-name이다.
    if (normalized == "MP4V-ES" || normalized == "MPEG4") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::MPEG4;
        profile.name = "MPEG4";
        profile.encoding_name = original;

        profile.depay_element = "rtpmp4vdepay";
        profile.parser_element = "mpeg4videoparse";
        profile.decoder_element = "avdec_mpeg4";

        profile.supported = false;
        profile.record_to_mp4_supported = false;
        profile.appsink_supported = false;

        return profile;
    }

    // 5. MPEG-2 Video RTP
    if (normalized == "MPV" || normalized == "MPEG2") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::MPEG2;
        profile.name = "MPEG2";
        profile.encoding_name = original;

        profile.depay_element = "rtpmpvdepay";
        profile.parser_element = "mpegvideoparse";
        profile.decoder_element = "avdec_mpeg2video";

        profile.supported = false;
        profile.record_to_mp4_supported = false;
        profile.appsink_supported = false;

        return profile;
    }

    // 6. MPEG-TS over RTP
    // MP2T는 내부에 다시 H264/H265 등이 들어있을 수 있으므로 별도 demux 설계가 필요하다.
    if (normalized == "MP2T") {
        VideoStreamProfile profile;

        profile.codec = VideoCodec::MPEG_TS;
        profile.name = "MPEG_TS";
        profile.encoding_name = original;

        profile.depay_element = "rtpmp2tdepay";
        profile.parser_element = "";
        profile.decoder_element = "";

        profile.supported = false;
        profile.record_to_mp4_supported = false;
        profile.appsink_supported = false;

        return profile;
    }

    return make_unknown_profile(original);
}

std::string video_codec_to_string(VideoCodec codec) {
    switch (codec) {
    case VideoCodec::H264:
        return "H264";

    case VideoCodec::H265:
        return "H265";

    case VideoCodec::MJPEG:
        return "MJPEG";

    case VideoCodec::MPEG4:
        return "MPEG4";

    case VideoCodec::MPEG2:
        return "MPEG2";

    case VideoCodec::MPEG_TS:
        return "MPEG_TS";

    case VideoCodec::Unknown:
    default:
        return "Unknown";
    }
}

bool is_supported_video_profile(const VideoStreamProfile& profile) {
    return profile.supported;
}