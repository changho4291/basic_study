#include <gst/gst.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    const std::string pipeline_desc = "videotestsrc num-buffers=30 ! fakesink";

    GError* error = nullptr;

    // 2. gst-launch에서 쓰던 파이프라인 문자열을 코드로 생성
    GstElement* pipeline = gst_parse_launch(pipeline_desc.c_str(), &error);

    if (error != nullptr) {
        std::cerr << "Pipeline parse error: " << error->message << std::endl;
        return 1;
    }

    if (pipeline == nullptr) {
        std::cerr << "Failed to create pipeline: " << std::endl;
        return 1;
    }

    // 3. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 4. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline);

    // 5. ERROR 또는 EOS 메시지가 올 때까지 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus, 
        GST_CLOCK_TIME_NONE, 
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    // 6. 메시지 처리
    if (msg != nullptr) {
        switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError* err = nullptr;
            gchar* debug_info = nullptr;

            gst_message_parse_error(msg, &err, &debug_info);

            std::cerr << "GStreamer ERROR: "
                        << (err ? err->message : "unknown err")
                        << std::endl;

            std::cerr << "Debug info: "
                        << (debug_info ? debug_info : "none")
                        << std::endl;

            if (err) { g_error_free(err); }

            if (debug_info) { g_free(debug_info); }

            break;
        }

        case GST_MESSAGE_EOS:
            std::cout << "Endd of stream" << std::endl;
            break;

        default:
            std::cout << "Unexpected message" << std::endl;
            break;
        }
    }

    // 7. 정리
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
