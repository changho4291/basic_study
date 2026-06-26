#include <gst/gst.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 저장 파일 패턴 설정
    std::string output_pattern = "snapshot_%03d.jpg";

    if (argc >= 2) { output_pattern = argv[1]; }

    std::cout << "Output pattern: " << output_pattern << std::endl;

    // 3. 파이프라인 생성
    GstElement* pipeline = gst_pipeline_new("jpeg-snapshot-pipeline");

    // 4. Element 생성
    GstElement* source = gst_element_factory_make("videotestsrc", "source");
    GstElement* convert = gst_element_factory_make("videoconvert", "convert");
    GstElement* encoder = gst_element_factory_make("jpegenc", "encoder");
    GstElement* sink = gst_element_factory_make("multifilesink", "sink");

    // 5. Element 생성 확인
    if (pipeline == nullptr || source == nullptr || convert == nullptr ||
        encoder == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one our more GStreamer elements" << std::endl;

        if (pipeline != nullptr) { gst_object_unref(pipeline); }

        return 1;
    }

    // 6. source 속성 설정
    g_object_set(
        source,
        "num-buffers", 1,
        nullptr
    );

    // 7. multifilesink 속성 설정
    g_object_set(
        sink,
        "location", output_pattern.c_str(),
        nullptr
    );

    // 8. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline),
        source,
        convert,
        encoder,
        sink,
        nullptr
    );

    // 9. Element 연결
    if (!gst_element_link_many(source, convert, encoder, sink, nullptr)) {
        std::cerr << "Failed to link elements" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 10. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 11. 실행 상태 확인
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 12. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline);

    // 13. ERROR 또는 EOS 메시지 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    // 14. 메시지 처리
    if (msg != nullptr) {
        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError* err = nullptr;
            gchar* debug_info = nullptr;

            gst_message_parse_error(msg, &err, &debug_info);

            std::cerr << "GStreamer ERROR: "
                      << (err ? err->message : "unknown error")
                      << std::endl;

            std::cerr << "Debug info: "
                      << (debug_info ? debug_info : "none")
                      << std::endl;

            if (err != nullptr) {
                g_error_free(err);
            }

            if (debug_info != nullptr) {
                g_free(debug_info);
            }

            break;
        }

        case GST_MESSAGE_EOS:
            std::cout << "End of stream" << std::endl;
            break;

        default:
            std::cout << "Unexpected message" << std::endl;
            break;
        }

        gst_message_unref(msg);
    }

    // 15. Bus 해제
    gst_object_unref(bus);

    // 16. 파이프라인 정지
    gst_element_set_state(pipeline, GST_STATE_NULL);

    // 17. 파이프라인 해제
    gst_object_unref(pipeline);

    std::cout << "Snapshot saved: " << output_pattern << std::endl;

    return 0;
}
