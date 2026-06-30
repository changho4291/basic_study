#ifndef __CAMERAWORKER_H__
#define __CAMERAWORKER_H__

#include <atomic>
#include <string>
#include <thread>

class CameraWorker {
private:
    std::string rtsp_uri_;
    std::string record_pattern_;
    int split_seconds_ = 5;

    std::atomic_bool stop_requested_{false};
    std::thread worker_thread_;

public:
    CameraWorker(
        std::string rtsp_uri,
        std::string record_pattern,
        int split_seconds
    );

    ~CameraWorker();

    CameraWorker(const CameraWorker&) = delete;
    CameraWorker& operator=(const CameraWorker&) = delete;

    void start();
    void stop();

private:
    void run();
    void sleep_retry_interval();
};

#endif // __CAMERAWORKER_H__