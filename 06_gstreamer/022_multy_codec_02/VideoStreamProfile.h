#ifndef __VIDEOSTREAMPROFILE_H__
#define __VIDEOSTREAMPROFILE_H__

#include <string>

enum class VideoCodec {
    Unknown,
    H264,
    H265,
    MJPEG,
    MPEG4,
    MPEG2,
    MPEG_TS
};

struct VideoStreamProfile {
    VideoCodec codec = VideoCodec::Unknown;

    std::string name;
    std::string encoding_name;

    std::string depay_element;
    std::string parser_element;
    std::string decoder_element;

    bool supported = false;
    bool record_to_mp4_supported = false;
    bool appsink_supported = false;
};

VideoStreamProfile make_video_stream_profile_from_encoding_name(
    const std::string& encoding_name
);

std::string video_codec_to_string(VideoCodec codec);

bool is_supported_video_profile(const VideoStreamProfile& profile);

#endif // __VIDEOSTREAMPROFILE_H__