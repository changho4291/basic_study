#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__

#include "CameraWorker.h"

#include <memory>
#include <mutex>
#include <string>
#include <vector>

struct CameraConfig {
    std::string camera_id;
    std::string rtsp_uri;
    std::string record_pattern;
    int split_seconds = 5;
};

class CameraManager {
public:
    CameraManager() = default;
    ~CameraManager();

    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

    bool add_camera(const CameraConfig& config);

    void start_all();
    void stop_all();

    std::size_t camera_count() const;
    bool is_running() const;

private:
    bool has_camera_id_locked(const std::string& camera_id) const;

private:
    bool running_ = false;

    mutable std::mutex mutex_;
    std::vector<std::unique_ptr<CameraWorker>> workers_;
};

#endif // __CAMERAMANAGER_H__