#include "CameraManager.h"

#include <iostream>

#include <iostream>

CameraManager::~CameraManager() {
    stop_all();
}

bool CameraManager::add_camera(const CameraConfig& config) {
    // 1. 필수 값 확인
    if (config.camera_id.empty()) {
        std::cerr << "camera_id is empty" << std::endl;
        return false;
    }

    if (config.rtsp_uri.empty()) {
        std::cerr << "rtsp_uri is empty" << std::endl;
        return false;
    }

    if (config.record_pattern.empty()) {
        std::cerr << "record_pattern is empty" << std::endl;
        return false;
    }

    // 2. CameraWorker 생성
    auto worker = std::make_unique<CameraWorker>(
        config.camera_id,
        config.rtsp_uri,
        config.record_pattern,
        config.split_seconds
    );

    // 3. Manager에 등록
    std::lock_guard<std::mutex> lock(mutex_);

    if (has_camera_id_locked(config.camera_id)) {
        std::cerr << "Camera already exists: "
                  << config.camera_id
                  << std::endl;
        return false;
    }

    CameraWorker* added_worker = worker.get();

    workers_.push_back(std::move(worker));

    std::cout << "Added camera: "
              << config.camera_id
              << std::endl;

    // 4. 이미 Manager가 실행 중이면 새 카메라도 즉시 시작
    if (running_) {
        std::cout << "Starting newly added camera: "
                  << config.camera_id
                  << std::endl;

        added_worker->start();
    }

    return true;
}

void CameraManager::start_all() {
    std::lock_guard<std::mutex> lock(mutex_);

    // 1. 이미 실행 중이면 무시
    if (running_) { return; }

    // 2. 실행 상태로 변경
    running_ = true;

    // 3. 모든 worker 시작
    for (auto& worker : workers_) {
        if (worker == nullptr) { continue; }

        std::cout << "Starting camera: "
                  << worker->camera_id()
                  << std::endl;

        worker->start();
    }
}

void CameraManager::stop_all() {
    std::lock_guard<std::mutex> lock(mutex_);

    // 1. 정지 상태로 변경
    running_ = false;

    // 2. 모든 worker 정지
    for (auto& worker : workers_) {
        if (worker == nullptr) { continue; }

        std::cout << "Stopping camera: "
                  << worker->camera_id()
                  << std::endl;

        worker->stop();
    }
}

std::size_t CameraManager::camera_count() const {
    std::lock_guard<std::mutex> lock(mutex_);

    return workers_.size();
}

bool CameraManager::is_running() const {
    std::lock_guard<std::mutex> lock(mutex_);

    return running_;
}

bool CameraManager::has_camera_id_locked(const std::string& camera_id) const {
    for (const auto& worker : workers_) {
        if (worker == nullptr) { continue; }

        if (worker->camera_id() == camera_id) { return true; }
    }

    return false;
}