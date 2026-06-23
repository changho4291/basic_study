#include <gst/gst.h>

#include <iostream>

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 파이프라인 생성
    GstElement* pipeline = gst_pipeline_new("manual-pipeline");

    // 3. Element 생성
    GstElement* source = gst_element_factory_make("videotestsrc", "source");
    GstElement* convert = gst_element_factory_make("videoconvert", "convert");
    GstElement* sink = gst_element_factory_make("fakesink", "sink");

    // 4. Element 생성 확인
    if (pipeline == nullptr || source == nullptr || convert == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;

        if (pipeline != nullptr) { gst_object_unref(pipeline); }

        return 1;
    }

    // 5. Element 속성 설정
    g_object_set (
        source,
        "num-buffers", 30,
        nullptr
    );

    // 6. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline),
        source,
        convert,
        sink,
        nullptr
    );

    // 7. Element 연결
    gboolean linked = gst_element_link_many(
        source,
        convert,
        sink,
        nullptr
    );

    // 8. Element 연결 확인
    if (!linked) {
        std::cerr << "Failed to link elements" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 9. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 10. 실행 상태 확인
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 11. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline);

    // 12. ERROR 또는 EOS 메시지 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    // 13. 메시지 처리
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
                      << (err ? err->message : "nunkown error")
                      << std::endl;

            if (err != nullptr) { g_error_free(err); }

            if (debug_info != nullptr) { g_free(debug_info); }

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

    // 14. Bus 해제
    gst_object_unref(bus);

    // 15. 파이프라인 정지
    gst_element_set_state(pipeline, GST_STATE_NULL);

    // 16. 파이프라인 해제
    gst_object_unref(pipeline);

    return 0;
}