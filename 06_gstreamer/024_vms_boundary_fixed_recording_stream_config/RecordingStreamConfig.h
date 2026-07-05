#pragma once

#include <string>

// GStreamer 녹화/수신 계층에서 사용할 코덱 힌트다.
// 이 값은 ONVIF가 직접 쓰는 타입이 아니라, VMS 녹화 엔진이 이해하는 내부 힌트다.
enum class StreamCodecHint {
    Unknown,
    H264,
    H265,
    MJPEG,
    MPEG4,
    MPEG2,
    MPEG_TS
};

// ONVIF, 자체 프로토콜, 수동 설정 등 외부 출처에서 온 스트림 식별 정보다.
// GStreamer 동작에는 필수 값이 아니며, 로그/DB/역추적용 메타데이터로만 사용한다.
struct RecordingStreamSourceRef {
    std::string source_type;        // 예: "onvif", "manual", "vendor"
    std::string external_device_id; // 예: ONVIF EndpointReference, 벤더 장비 ID
    std::string external_stream_id; // 예: ONVIF media profile token, 벤더 stream id
};

// GStreamer 녹화 파이프라인을 실행하기 위한 VMS 내부 설정 객체다.
// 주의: 이 구조체는 ONVIF Media Profile이 아니다.
// ONVIF DTO는 별도 모듈에 두고, mapper를 통해 이 구조체로 변환해서 사용한다.
struct RecordingStreamConfig {
    // 1. VMS 내부 카메라/스트림 식별자
    std::string camera_id;
    std::string stream_id;
    std::string stream_name;

    // 2. 실제 GStreamer rtspsrc에 넘길 주소
    std::string rtsp_uri;

    // 3. 외부 프로토콜 원본 식별자 추적용 메타데이터
    RecordingStreamSourceRef source_ref;

    // 4. VMS가 알고 있는 스트림 힌트
    //    실제 RTP caps는 CameraPipeline에서 다시 검증한다.
    StreamCodecHint codec_hint = StreamCodecHint::Unknown;
    int width = 0;
    int height = 0;
    int fps = 0;
    int bitrate_kbps = 0;

    // 5. VMS 용도 구분
    bool is_main_stream = false;
    bool is_sub_stream = false;
    bool record_enabled = true;
    bool live_view_enabled = true;

    // 6. 녹화 정책
    std::string record_pattern;
    int split_seconds = 5;
};

std::string stream_codec_hint_to_string(StreamCodecHint codec_hint);
std::string describe_recording_stream_config(const RecordingStreamConfig& config);
