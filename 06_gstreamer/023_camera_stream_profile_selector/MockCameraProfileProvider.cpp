#include "MockCameraProfileProvider.h"

#include <iostream>

std::vector<CameraStreamProfile> MockCameraProfileProvider::get_stream_profiles(
    const std::string& camera_id
) {
    // 1. 실제 VMS라면 여기서 ONVIF GetProfiles/GetStreamUri 또는 자체 API를 호출한다.
    //    지금은 강의 흐름을 위해 Main/Sub 두 개의 가짜 profile을 반환한다.
    std::vector<CameraStreamProfile> profiles;

    profiles.push_back(make_main_stream_profile(camera_id));
    profiles.push_back(make_sub_stream_profile(camera_id));

    std::cout << "Mock profile provider returned "
              << profiles.size()
              << " profiles for camera_id="
              << camera_id
              << std::endl;

    return profiles;
}

CameraStreamProfile MockCameraProfileProvider::make_main_stream_profile(
    const std::string& camera_id
) const {
    CameraStreamProfile profile;

    // 1. 카메라/프로파일 식별 정보
    profile.camera_id = camera_id;
    profile.profile_token = "profile_main_001";
    profile.profile_name = "Main Stream";

    // 2. 실제 테스트용 RTSP URI
    //    기존 강의에서 사용하던 주소를 그대로 유지한다.
    profile.rtsp_uri = "rtsp://127.0.0.1:8554/test";

    // 3. 카메라가 알려준다고 가정하는 스트림 속성
    profile.codec_hint = StreamCodecHint::H264;
    profile.width = 1920;
    profile.height = 1080;
    profile.fps = 30;
    profile.bitrate_kbps = 4096;

    // 4. VMS 용도 정보
    profile.is_main_stream = true;
    profile.is_sub_stream = false;
    profile.record_enabled = true;
    profile.live_view_enabled = true;

    // 5. 녹화 설정
    profile.record_pattern = "record_" + camera_id + "_main_%03d.mp4";
    profile.split_seconds = 5;

    return profile;
}

CameraStreamProfile MockCameraProfileProvider::make_sub_stream_profile(
    const std::string& camera_id
) const {
    CameraStreamProfile profile;

    // 1. 카메라/프로파일 식별 정보
    profile.camera_id = camera_id;
    profile.profile_token = "profile_sub_001";
    profile.profile_name = "Sub Stream";

    // 2. 테스트용 Sub Stream RTSP URI
    //    실제 테스트 서버에 이 경로가 없다면 이번 강의에서는 선택하지 않는다.
    profile.rtsp_uri = "rtsp://127.0.0.1:8554/test_sub";

    // 3. 카메라가 알려준다고 가정하는 스트림 속성
    profile.codec_hint = StreamCodecHint::H264;
    profile.width = 640;
    profile.height = 360;
    profile.fps = 15;
    profile.bitrate_kbps = 512;

    // 4. VMS 용도 정보
    profile.is_main_stream = false;
    profile.is_sub_stream = true;
    profile.record_enabled = false;
    profile.live_view_enabled = true;

    // 5. Sub Stream은 이번 단계에서 녹화하지 않지만, 구조상 값은 채워둔다.
    profile.record_pattern = "record_" + camera_id + "_sub_%03d.mp4";
    profile.split_seconds = 5;

    return profile;
}
