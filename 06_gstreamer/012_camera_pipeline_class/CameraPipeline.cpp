#include "CameraPipeline.h"

CameraPipeline::CameraPipeline(std::string rtsp_uri) : rtsp_uri_(std::move(rtsp_uri)) {}

CameraPipeline::~CameraPipeline() { stop(); }

bool CameraPipeline::create() {
    // 1. 기존 파이프라인 정리
    stop();

    // 2. 파이프라인 생성
    pipeline_ = gst_pipeline_new("camera-pipeline");

    // 3. Element 생성
    GstElement* source = gst_element_factory_make("rtspsrc", "source");
    GstElement* depay = gst_element_factory_make("rtph264depay", "depay");
    GstElement* parser = gst_element_factory_make("h264parse", "parser");
    GstElement* sink = gst_element_factory_make("fakesink", "sink");

    // 4. Element 생성 확인
    if (pipeline_ == nullptr || source == nullptr || depay == nullptr ||
        parser == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;

        if (source != nullptr) {
            gst_object_unref(source);
        }

        if (depay != nullptr) {
            gst_object_unref(depay);
        }

        if (parser != nullptr) {
            gst_object_unref(parser);
        }

        if (sink != nullptr) {
            gst_object_unref(sink);
        }

        if (pipeline_ != nullptr) {
            gst_object_unref(pipeline_);
            pipeline_ = nullptr;
        }

        depay_ = nullptr;
        return false;
    }

    // 5. Element 속성 설정
    g_object_set(
        source,
        "location", rtsp_uri_.c_str(),
        "latency", 100,
        nullptr
    );

    g_object_set(
        sink,
        "sync", FALSE,
        nullptr
    );

    // 6. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline_),
        source,
        depay,
        parser,
        sink,
        nullptr
    );

    // 7. 정적 구간 연결
    if (!gst_element_link_many(depay, parser, sink, nullptr)) {
        std::cerr << "Failed to link depay, parser, sink" << std::endl;
        stop();
        return false;
    }

    // 8. Dynamic Pad 콜백 등록
    depay_ = depay;

    g_signal_connect(
        source,
        "pad-added",
        G_CALLBACK(&CameraPipeline::on_pad_added),
        this
    );

    return true;
}


bool CameraPipeline::start() {
    // 1. 파이프라인 생성 여부 확인
    if (pipeline_ == nullptr) {
        std::cerr << "Pipeline is not created" << std::endl;
        return false;
    }

    // 2. PLAYING 상태로 전환
    GstStateChangeReturn ret = gst_element_set_state(
        pipeline_,
        GST_STATE_PLAYING
    );

    // 3. 실행 상태 확인
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        return false;
    }

    return true;
}

bool CameraPipeline::wait_until_error_or_eos() {
    // 1. 파이프라인 생성 여부 확인
    if (pipeline_ == nullptr) {
        return false;
    }

    // 2. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline_);

    if (bus == nullptr) {
        return false;
    }

    // 3. ERROR 또는 EOS 메시지 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    bool need_reconnect = true;

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

            need_reconnect = true;
            break;
        }

        case GST_MESSAGE_EOS:
            std::cout << "End of stream" << std::endl;
            need_reconnect = true;
            break;

        default:
            need_reconnect = true;
            break;
        }

        gst_message_unref(msg);
    }

    // 5. Bus 해제
    gst_object_unref(bus);

    return need_reconnect;
}

void CameraPipeline::stop() {
    // 1. 파이프라인 존재 여부 확인
    if (pipeline_ == nullptr) {
        depay_ = nullptr;
        return;
    }

    // 2. 파이프라인 정지
    gst_element_set_state(pipeline_, GST_STATE_NULL);

    // 3. 파이프라인 해제
    gst_object_unref(pipeline_);

    pipeline_ = nullptr;
    depay_ = nullptr;
}

void CameraPipeline::on_pad_added(
    GstElement* src,
    GstPad* new_pad,
    gpointer user_data
) {
    (void)src;

    CameraPipeline* self = static_cast<CameraPipeline*>(user_data);

    if (self == nullptr) {
        return;
    }

    self->handle_pad_added(new_pad);
}

void CameraPipeline::handle_pad_added(GstPad* new_pad) {
    // 1. rtph264depay의 sink pad 가져오기
    GstPad* sink_pad = gst_element_get_static_pad(depay_, "sink");

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

        if (caps != nullptr) {
            gst_caps_unref(caps);
        }

        gst_object_unref(sink_pad);
        return;
    }

    // 4. RTP pad인지 확인
    GstStructure* structure = gst_caps_get_structure(caps, 0);
    const gchar* pad_type = gst_structure_get_name(structure);

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

    if (encoding_name == nullptr ||
        g_ascii_strcasecmp(encoding_name, "H264") != 0) {
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