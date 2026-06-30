#include "CameraWorker.h"

#include <gst/gst.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 실행 인자 설정
    std::string rtsp_uri = "rtsp://127.0.0.1:8554/test";
    std::string record_pattern = "record_%03d.mp4";
    int split_seconds = 5;

    if (argc >= 2) { rtsp_uri = argv[1]; }
    if (argc >= 3) { record_pattern = argv[2]; }
    if (argc >= 4) { split_seconds = std::stoi(argv[3]); }

    std::cout << "RTSP URI: " << rtsp_uri << std::endl;
    std::cout << "Record pattern: " << record_pattern << std::endl;
    std::cout << "Split seconds: " << split_seconds << std::endl;

    // 3. CameraWorker 생성
    CameraWorker worker(
        rtsp_uri,
        record_pattern,
        split_seconds
    );

    // 4. worker 시작
    worker.start();

    std::cout << "CameraWorker started" << std::endl;
    std::cout << "Press Enter to stop..." << std::endl;

    // 5. 사용자 입력 대기
    std::cin.get();

    // 6. worker 정지
    worker.stop();

    std::cout << "Program finished" << std::endl;

    return 0;
}
