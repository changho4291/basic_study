#ifndef __CAMERAWORKER_H__
#define __CAMERAWORKER_H__

#include <atomic>
#include <string>
#include <thread>

class CameraWorker {
public:
    CameraWorker(
        std::string camera_id,
        std::string rtsp_uri,
        std::string record_pattern,
        int split_seconds
    );

    ~CameraWorker();

    CameraWorker(const CameraWorker&) = delete;
    CameraWorker& operator=(const CameraWorker&) = delete;

    void start();
    void stop();

    const std::string& camera_id() const;

private:
    void run();
    void sleep_retry_interval();

private:
    std::string camera_id_;
    std::string rtsp_uri_;
    std::string record_pattern_;
    int split_seconds_ = 5;

    std::atomic_bool stop_requested_{false};
    std::thread worker_thread_;
};

#endif // __CAMERAWORKER_H__