#ifndef __MOCKCAMERAPROFILEPROVIDER_H__
#define __MOCKCAMERAPROFILEPROVIDER_H__

#include "CameraProfileProvider.h"

#include <string>
#include <vector>

// ONVIF 또는 실제 카메라 API를 붙이기 전까지 사용하는 가짜 프로파일 제공자.
// VMS의 상위 구조를 먼저 만들기 위한 학습용 구현체다.
class MockCameraProfileProvider : public CameraProfileProvider {
public:
    std::vector<CameraStreamProfile> get_stream_profiles(
        const std::string& camera_id
    ) override;

private:
    CameraStreamProfile make_main_stream_profile(
        const std::string& camera_id
    ) const;

    CameraStreamProfile make_sub_stream_profile(
        const std::string& camera_id
    ) const;
};

#endif // __MOCKCAMERAPROFILEPROVIDER_H__
