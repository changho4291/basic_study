#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__

#include "CameraWorker.h"

#include <memory>
#include <string>
#include <vector>

struct CameraConfig {
    std::string camera_id;
    std::string rtsp_uri;
    std::string record_pattern;
    int split_secondds = 5;
};

class CameraManager {
private:
    bool running_ = false;
    std::vector<std::unique_ptr<CameraWorker>> workers_;

public:
    CameraManager() = default;
    ~CameraManager();

    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    bool add_camera(const CameraConfig& config);

    void start_all();
    void stop_all();

    std::size_t camera_count() const;
};

#endif // __CAMERAMANAGER_H__