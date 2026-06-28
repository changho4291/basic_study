#include "CameraPipeline.h"

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

    if (argc >= 2) {
        rtsp_uri = argv[1];
    }

    if (argc >= 3) {
        record_pattern = argv[2];
    }

    if (argc >= 4) {
        split_seconds = std::stoi(argv[3]);
    }

    std::cout << "RTSP URI: " << rtsp_uri << std::endl;
    std::cout << "Record pattern: " << record_pattern << std::endl;
    std::cout << "Split seconds: " << split_seconds << std::endl;

    int retry_count = 0;

    // 3. 재연결 루프
    while (true) {
        retry_count++;

        std::cout << std::endl;
        std::cout << "===== Connect attempt " << retry_count << " =====" << std::endl;

        // 4. CameraPipeline 객체 생성
        CameraPipeline camera(
            rtsp_uri,
            record_pattern,
            split_seconds
        );

        // 5. 파이프라인 생성
        if (!camera.create()) {
            std::cerr << "Failed to create camera pipeline" << std::endl;
            std::cout << "Retry after 3 seconds..." << std::endl;
            g_usleep(3 * G_USEC_PER_SEC);
            continue;
        }

        // 6. 파이프라인 실행
        if (!camera.start()) {
            std::cerr << "Failed to start camera pipeline" << std::endl;
            std::cout << "Retry after 3 seconds..." << std::endl;
            g_usleep(3 * G_USEC_PER_SEC);
            continue;
        }

        // 7. ERROR 또는 EOS 대기
        bool need_reconnect = camera.wait_until_error_or_eos();

        // 8. 파이프라인 정리
        camera.stop();

        if (!need_reconnect) {
            break;
        }

        // 9. 재연결 대기
        std::cout << "Pipeline stopped. Retry after 3 seconds..." << std::endl;
        g_usleep(3 * G_USEC_PER_SEC);
    }

    return 0;
}