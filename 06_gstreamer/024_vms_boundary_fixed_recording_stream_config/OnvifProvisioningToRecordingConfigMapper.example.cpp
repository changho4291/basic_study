#include "OnvifProvisioningToRecordingConfigMapper.example.h"

#include <string>

namespace {

StreamCodecHint guess_codec_hint_from_profile_name(const std::string& name) {
    // 현재 사용자가 보여준 OnvifMediaProfile DTO에는 encoder 설정 상세값이 없다.
    // 따라서 이 함수는 임시 fallback 예시일 뿐이다.
    // 나중에 VideoEncoderConfiguration.Encoding을 DTO에 추가하면 그 값을 기준으로 변환해야 한다.
    if (name.find("265") != std::string::npos ||
        name.find("HEVC") != std::string::npos ||
        name.find("hevc") != std::string::npos) {
        return StreamCodecHint::H265;
    }

    if (name.find("264") != std::string::npos ||
        name.find("H264") != std::string::npos ||
        name.find("h264") != std::string::npos) {
        return StreamCodecHint::H264;
    }

    return StreamCodecHint::Unknown;
}

bool looks_like_main_stream(const std::string& name, std::size_t index) {
    if (index == 0) { return true; }

    return name.find("main") != std::string::npos ||
           name.find("Main") != std::string::npos ||
           name.find("MAIN") != std::string::npos;
}

bool looks_like_sub_stream(const std::string& name, std::size_t index) {
    if (index == 1) { return true; }

    return name.find("sub") != std::string::npos ||
           name.find("Sub") != std::string::npos ||
           name.find("SUB") != std::string::npos;
}

} // namespace

std::vector<RecordingStreamConfig> OnvifProvisioningToRecordingConfigMapper::map(
    const ProvisioningResult& provisioning_result
) const {
    std::vector<RecordingStreamConfig> configs;

    const std::string camera_id = provisioning_result.discoveredDevice.endpointReference;

    for (std::size_t i = 0; i < provisioning_result.profiles.size(); ++i) {
        const OnvifProvisionedProfile& provisioned_profile = provisioning_result.profiles[i];

        if (!provisioned_profile.streamUri.has_value()) {
            continue;
        }

        RecordingStreamConfig config;

        config.camera_id = camera_id;
        config.stream_id = "onvif_" + provisioned_profile.profile.token;
        config.stream_name = provisioned_profile.profile.name;
        config.rtsp_uri = provisioned_profile.streamUri->uri;

        // 외부 출처 추적용. GStreamer는 이 값을 쓰지 않는다.
        config.source_ref.source_type = "onvif";
        config.source_ref.external_device_id = provisioning_result.discoveredDevice.endpointReference;
        config.source_ref.external_stream_id = provisioned_profile.profile.token;

        // 현재 사용자가 보여준 OnvifMediaProfile에는 해상도/코덱/fps/bitrate가 없다.
        // 그래서 일단 Unknown/0으로 두고, 나중에 VideoEncoderConfiguration DTO를 추가하면 여기서 채운다.
        config.codec_hint = guess_codec_hint_from_profile_name(provisioned_profile.profile.name);
        config.width = 0;
        config.height = 0;
        config.fps = 0;
        config.bitrate_kbps = 0;

        config.is_main_stream = looks_like_main_stream(config.stream_name, i);
        config.is_sub_stream = looks_like_sub_stream(config.stream_name, i);

        config.record_enabled = config.is_main_stream;
        config.live_view_enabled = true;

        config.record_pattern = "record_" + config.camera_id + "_" + config.stream_id + "_%03d.mp4";
        config.split_seconds = 5;

        configs.push_back(config);
    }

    return configs;
}
