#include <gst/gst.h>

#include <iostream>
#include <string>

struct AppData {
    GstElement* depay = nullptr;
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
        gst_object_unref(sink_pad);
        return;
    }

    // 3. 새 pad의 caps 확인
    GstCaps* caps = gst_pad_get_current_caps(new_pad);

    if (caps == nullptr) {
        caps = gst_pad_query_caps(new_pad, nullptr);
    }

    if (caps == nullptr || gst_caps_get_size(caps) == 0) {
        std::cerr << "Failed to get new pad caps" << std::endl;

        if (caps != nullptr) { gst_caps_unref(caps); }

        gst_object_unref(sink_pad);
        return;
    }

    GstStructure* structure = gst_caps_get_structure(caps, 0);
    const gchar* pad_type = gst_structure_get_name(structure);

    // 4. RTP pad인지 확인
    if (g_str_has_prefix(pad_type, "application/x-rtp") == FALSE) {
        gst_caps_unref(caps);
        gst_object_unref(sink_pad);
        return;
    }

    // 5. H264 RTP인지 확인
    const gchar* encoding_name = gst_structure_get_string(
        structure,
        "encoding-name"
    );

    std::cout << "RTP encoding-name: "
              << (encoding_name ? encoding_name : "none")
              << std::endl;

    if (encoding_name == nullptr || g_ascii_strcasecmp(encoding_name, "H264") != 0) {
        std::cout << "It is not H264 RTP. Ignore this pad." << std::endl;

        gst_caps_unref(caps);
        gst_object_unref(sink_pad);
        return;
    }

    // 6. rtspsrc의 동적 pad를 rtph264depay에 연결
    GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);

    if (GST_PAD_LINK_FAILED(ret)) {
        std::cerr << "Failed to link rtspsrc dynamic pad" << std::endl;
    } else {
        std::cout << "RTSP dynamic pad linked successfully" << std::endl;
    }

    // 7. 참조 해제
    gst_caps_unref(caps);
    gst_object_unref(sink_pad);
}

static GstElement* create_pipeline(const std::string& rtsp_uri, AppData* data) {
    // 1. 파이프라인 생성
    GstElement* pipeline = gst_pipeline_new("rtsp-reconnect-pipeline");

    // 2. Element 생성
    GstElement* source = gst_element_factory_make("rtspsrc", "source");
    GstElement* depay = gst_element_factory_make("rtph264depay", "depay");
    GstElement* parser = gst_element_factory_make("h264parse", "parser");
    GstElement* sink = gst_element_factory_make("fakesink", "sink");

    // 3. Element 생성 확인
    if (pipeline == nullptr || source == nullptr || depay == nullptr ||
        parser == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;

        if (pipeline != nullptr) {
            gst_object_unref(pipeline);
        }

        return nullptr;
    }

    // 4. Element 속성 설정
    g_object_set(
        source,
        "location", rtsp_uri.c_str(),
        "latency", 100,
        nullptr
    );

    g_object_set(
        sink,
        "sync", FALSE,
        nullptr
    );

    // 5. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline),
        source,
        depay,
        parser,
        sink,
        nullptr
    );

    // 6. 정적 구간 연결
    if (!gst_element_link_many(depay, parser, sink, nullptr)) {
        std::cerr << "Failed to link depay, parser, sink" << std::endl;
        gst_object_unref(pipeline);
        return nullptr;
    }

    // 7. Dynamic Pad 콜백 등록
    data->depay = depay;

    g_signal_connect(
        source,
        "pad-added",
        G_CALLBACK(on_pad_added),
        data
    );

    return pipeline;
}

static bool run_pipeline_until_error_or_eos(GstElement* pipeline) {
    // 1. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        return false;
    }

    // 2. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline);

    // 3. ERROR 또는 EOS 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    bool reconnect = true;

    // 4. 메시지 처리
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

            reconnect = true;
            break;
        }

        case GST_MESSAGE_EOS:
            std::cout << "End of stream" << std::endl;
            reconnect = true;
            break;

        default:
            reconnect = true;
            break;
        }

        gst_message_unref(msg);
    }

    // 5. Bus 해제
    gst_object_unref(bus);

    return reconnect;
}

static void destroy_pipeline(GstElement* pipeline) {
    if (pipeline == nullptr) {
        return;
    }

    // 1. 파이프라인 정지
    gst_element_set_state(pipeline, GST_STATE_NULL);

    // 2. 파이프라인 해제
    gst_object_unref(pipeline);
}

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. RTSP URI 설정
    std::string rtsp_uri = "rtsp://127.0.0.1:8554/test";

    if (argc >= 2) { rtsp_uri = argv[1]; }

    std::cout << "RTSP URI" << rtsp_uri << std::endl;

    int retry_count = 0;

    // 3. 재연결 루프
    while (true) {
        retry_count++;

        std::cout << std::endl;
        std::cout << "==== Connect attempt " << retry_count << " ====" << std::endl;

        AppData data;

        // 4. 파이프라인 생성
        GstElement* pipeline = create_pipeline(rtsp_uri, &data);

        if (pipeline == nullptr) {
            std::cerr << "Failed to create pipeline" << std::endl;
            std::cout << "Retry after 3 seconds..." << std::endl;
            g_usleep(3 * G_USEC_PER_SEC);
            continue;
        }

        // 5. 실행 후 ERROR/EOS까지 대기
        bool reconnect = run_pipeline_until_error_or_eos(pipeline);

        // 6. 파이프라인 정리
        destroy_pipeline(pipeline);

        if (!reconnect) { break; }

        // 7. 재연결 대기
        std::cout << "Pipeline stopped. Retry after 3 seconds..." << std::endl;
        g_usleep(3 * G_USEC_PER_SEC);
    }

    return 0;
}
