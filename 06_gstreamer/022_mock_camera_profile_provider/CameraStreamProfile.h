#ifndef __CAMERASTREAMPROFILE_H__
#define __CAMERASTREAMPROFILE_H__

#include <string>

// VMS가 카메라로부터 조회하거나 설정으로 보유하는 코덱 힌트.
// 실제 RTP caps는 나중에 GStreamer pad-added에서 다시 검증한다.
enum class StreamCodecHint {
    Unknown,
    H264,
    H265,
    MJPEG,
    MPEG4,
    MPEG2,
    MPEG_TS
};

struct CameraStreamProfile {
    // 1. 카메라 식별 정보
    std::string camera_id;

    // 2. ONVIF ProfileToken 또는 자체 프로토콜의 profile id
    std::string profile_token;

    // 3. 사용자에게 보여줄 profile 이름
    std::string profile_name;

    // 4. 선택된 profile의 RTSP URI
    std::string rtsp_uri;

    // 5. 카메라/ONVIF/자체 프로토콜에서 알려준 코덱 힌트
    StreamCodecHint codec_hint = StreamCodecHint::Unknown;

    // 6. 스트림 속성 정보
    int width = 0;
    int height = 0;
    int fps = 0;
    int bitrate_kbps = 0;

    // 7. VMS 용도 구분
    bool is_main_stream = false;
    bool is_sub_stream = false;

    // 8. 녹화 정책
    bool record_enabled = true;
    bool live_view_enabled = true;

    std::string record_pattern;
    int split_seconds = 5;
};

std::string stream_codec_hint_to_string(StreamCodecHint codec_hint);
std::string describe_camera_stream_profile(const CameraStreamProfile& profile);

#endif // __CAMERASTREAMPROFILE_H__
