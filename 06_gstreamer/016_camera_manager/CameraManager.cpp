#include "CameraManager.h"

#include <iostream>

CameraManager::~CameraManager() { stop_all(); }

bool CameraManager::add_camera(const CameraConfig& config) {
    // 1. 실행 중에는 카메라 추가 금지
    if (running_) {
        std::cerr << "Cannot add camera while manager is running" << std::endl;
        return false;
    }

    // 2. 필수 값 확인
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

    // 3. CameraWorker 생성
    auto worker = std::make_unique<CameraWorker>(
        config.camera_id,
        config.rtsp_uri,
        config.record_pattern,
        config.split_secondds
    );

    // 4. 목록에 추가
    workers_.push_back(std::move(worker));

    std::cout << "Added camera: " << config.camera_id << std::endl;

    return true;
}

void CameraManager::start_all() {
    // 1. 이미 실행 중이면 부시
    if (running_) { return; }

    // 2. 모든 worker 시작
    for (auto& worker : workers_) {
        if (worker == nullptr) { continue; }

        std::cout << "Starting camera:"
                  << worker->camera_id()
                  << std::endl;

        worker->start();
    }

    running_ = true;
}

void CameraManager::stop_all() {
    // 1. 실행 중이 아니어도 stop은 안전하게 호출 가능
    for (auto& worker : workers_) {
        if (worker == nullptr) { continue; }

        std::cout << "Stopping camera: "
                  << worker->camera_id()
                  << std::endl;

        worker->stop();
    }

    running_ = false;
}

std::size_t CameraManager::camera_count() const {
    return workers_.size();
}