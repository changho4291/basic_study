#pragma once

// 이 파일은 예시다.
// 실제 프로젝트에서는 ONVIF 모듈 쪽 include 경로가 잡힌 integration layer에 두는 것을 권장한다.
// 이 예시 파일은 CMake 빌드 대상에 포함하지 않았다.

#include "RecordingStreamConfig.h"
#include "OnvifProvisioningTypes.h"

#include <vector>

class OnvifProvisioningToRecordingConfigMapper {
public:
    std::vector<RecordingStreamConfig> map(
        const ProvisioningResult& provisioning_result
    ) const;
};
