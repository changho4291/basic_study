#include <gst/gst.h>

#include <iostream>
#include <string>

struct AppData {
    GstElement* depay;
};

static void on_pad_added(GstElement* src, GstPad* new_pad, gpointer user_data) {
    (void)src;

    AppData* data = static_cast<AppData*>(user_data);

    // 1. rtph264depay의 sink pad 가져오기
    GstPad* sink_pad = gst_element_get_static_pad(data->depay, "sink");

    if (sink_pad == nullptr) {
        std::cerr << "Failed to get depay sink pad" << std::endl;
        return;
    }

    // 2. 이미 연결되어 있으면 무시
    if (gst_pad_is_linked(sink_pad)) {
        std::cout << "Depay sink pad is already linked" << std::endl;
        gst_object_unref(sink_pad);
        return;
    }

    // 3. 새 pad의 caps 확인
    GstCaps* new_pad_caps = gst_pad_get_current_caps(new_pad);
    if (new_pad_caps == nullptr) {
        new_pad_caps = gst_pad_query_caps(new_pad, nullptr);
    }

    if (new_pad_caps == nullptr || gst_caps_get_size(new_pad_caps) == 0) {
        std::cerr << "Failed to get new pad caps" << std::endl;

        if (new_pad_caps != nullptr) { gst_caps_unref(new_pad_caps); }

        gst_object_unref(sink_pad);
        return;
    }

    GstStructure* new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    const gchar* new_pad_type = gst_structure_get_name(new_pad_struct);

    std::cout << "New pad type: " << new_pad_type << std::endl;

    // 4. application/x-rtp가 아니면 연결하지 않음
    if (g_str_has_prefix(new_pad_type, "application/x-rtp") == FALSE) {
        std::cout << "It is not RTP. Ignore this pad." << std::endl;

        gst_caps_unref(new_pad_caps);
        gst_object_unref(sink_pad);
        return;
    }

    // 5. RTP encoding-name 확인
    const gchar* encoding_name = nullptr;
    gst_structure_get(new_pad_struct, "encoding-name", G_TYPE_STRING
        , &encoding_name, nullptr);

    if (encoding_name == nullptr || g_strcmp0(encoding_name, "H264") != 0) {
        std::cout << "It is not H264 RTP. Ignore this pad." << std::endl;

        gst_caps_unref(new_pad_caps);
        gst_object_unref(sink_pad);
        return;
    }

    std::cout << "RTP encoding-name: " << encoding_name << std::endl;

    // 6. rtspsrc의 새 src pad를 rtph264depay의 sink pad에 연결
    GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);

    if (GST_PAD_LINK_FAILED(ret)) {
        std::cerr << "Failed to link rtspsrc dynamic pad" << std::endl;
    } else {
        std::cout << "RTSP dynamic pad linked successfully" << std::endl;
    }

    // 7. 참조 해제
    gst_caps_unref(new_pad_caps);
    gst_object_unref(sink_pad);
}

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. RTSP URI 설정
    std::string rtsp_uri = "rtsp://127.0.0.1:8554/test";

    if (argc >= 2) { rtsp_uri = argv[1]; }

    std::cout << "RTSP URI: " << rtsp_uri << std::endl;

    // 3. 파이프라인 생성
    GstElement* pipeline = gst_pipeline_new("rtsp-client-pipeline");

    // 4. Element 생성
    GstElement* source = gst_element_factory_make("rtspsrc", "source");
    GstElement* depay = gst_element_factory_make("rtph264depay", "depay");
    GstElement* parser = gst_element_factory_make("h264parse", "parser");
    GstElement* sink = gst_element_factory_make("fakesink", "sink");

    // 5. Element 생성 확인
    if (pipeline == nullptr || source == nullptr || depay == nullptr ||
        parser == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;

        if (pipeline != nullptr) { gst_object_unref(pipeline); }

        return 1;
    }

    // 6. Element 속성 설정
    g_object_set(
        source,
        "location", rtsp_uri.c_str(),
        "latency", 100,
        nullptr
    );

    // 7. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline),
        source,
        depay,
        parser,
        sink,
        nullptr
    );

    // 8. 정적 Pad 구간 연결
    if (!gst_element_link_many(depay, parser, sink, nullptr)) {
        std::cerr << "Failed to link depay, parser, sink" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 9. Dynamic Pad 콜백 등록
    AppData data;
    data.depay = depay;

    g_signal_connect(
        source,
        "pad-added",
        G_CALLBACK(on_pad_added),
        &data
    );

    // 10. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 11. 실행 상태 확인
    if (ret = GST_STATE_CHANGE_FAILURE) {
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

            if (err != nullptr) { g_error_free(err); }

            if (debug_info != nullptr) { g_free(debug_info); }

            break;
        }

        case GST_MESSAGE_EOS:
            std::cout << "End to stream" << std::endl;
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

    return 0;
}
