#ifndef __CAMERAWORKER_H__
#define __CAMERAWORKER_H__

#include <atomic>
#include <string>
#include <thread>

#include "CameraStreamProfile.h"
#include "RecordingSegmentStore.h"

class CameraWorker {
private:
    CameraStreamProfile stream_profile_;

    std::string camera_id_;

    RecordingSegmentStore* segment_store_ = nullptr;

    std::atomic_bool stop_requested_{false};
    std::thread worker_thread_;

public:
    CameraWorker(
        CameraStreamProfile stream_profile,
        RecordingSegmentStore* segment_store
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
};

#endif // __CAMERAWORKER_H__