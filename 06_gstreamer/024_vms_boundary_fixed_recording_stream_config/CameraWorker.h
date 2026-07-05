#ifndef __CAMERAWORKER_H__
#define __CAMERAWORKER_H__

#include <atomic>
#include <string>
#include <thread>

#include "RecordingStreamConfig.h"
#include "RecordingSegmentStore.h"

class CameraWorker {
private:
    RecordingStreamConfig stream_config_;

    std::string camera_id_;

    RecordingSegmentStore* segment_store_ = nullptr;

    std::atomic_bool stop_requested_{false};
    std::thread worker_thread_;

public:
    CameraWorker(
        RecordingStreamConfig stream_config,
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