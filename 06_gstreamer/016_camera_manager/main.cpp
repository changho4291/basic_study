#include "CameraManager.h"

#include <gst/gst.h>

#include <iostream>

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. CameraManager 생성
    CameraManager manager;

    // 3. 카메라 등록
    manager.add_camera({
        "camera_001",
        "rtsp://127.0.0.1:8554/test",
        "record_camera_001_%03d.mp4",
        5
    });

    manager.add_camera({
        "camera_002",
        "rtsp://127.0.0.1:8554/test",
        "record_camera_002_%03d.mp4",
        5
    });

    std::cout << "Camera count: "
              << manager.camera_count()
              << std::endl;

    // 4. 모든 카메라 시작
    manager.start_all();

    std::cout << "CameraManager started" << std::endl;
    std::cout << "Press Enter to stop..." << std::endl;

    // 5. 종료 입력 대기
    std::cin.get();

    // 6. 모든 카메라 정지
    manager.stop_all();

    std::cout << "Program finished" << std::endl;

    return 0;
}
