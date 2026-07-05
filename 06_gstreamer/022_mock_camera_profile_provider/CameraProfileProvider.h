#ifndef __CAMERAPROFILEPROVIDER_H__
#define __CAMERAPROFILEPROVIDER_H__

#include "CameraStreamProfile.h"

#include <string>
#include <vector>

// 카메라에서 스트림 프로파일 목록을 가져오는 추상 인터페이스.
// 지금은 Mock 구현체를 사용하지만, 나중에는 ONVIF 또는 자체 프로토콜 구현체로 교체한다.
class CameraProfileProvider {
public:
    virtual ~CameraProfileProvider() = default;

    virtual std::vector<CameraStreamProfile> get_stream_profiles(
        const std::string& camera_id
    ) = 0;
};

#endif // __CAMERAPROFILEPROVIDER_H__
