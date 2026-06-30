#include "CameraPipeline.h"

CameraPipeline::CameraPipeline(
    std::string rtsp_uri,
    std::string record_pattern,
    int spilt_seconds
) 
    : rtsp_uri_(std::move(rtsp_uri)), 
      record_pattern_(std::move(record_pattern)),
      split_seconds_(split_seconds_) {}

CameraPipeline::~CameraPipeline() {
    stop();
}

bool CameraPipeline::create() {
    // 1. 기존 파이프라인 정리
    stop();

    // 2. 파이프라인 생성
    pipeline_ = gst_pipeline_new("camera-record-pipeline");

    if (pipeline_ == nullptr) {
        std::cerr << "Failed to create pipeline" << std::endl;
        return false;
    }

    // 3. Element 생성
    if (!create_elements()) {
        stop();
        return false;
    }

    // 4. Element 속성 설정
    if (!configure_elements()) {
        stop();
        return false;
    }

    // 5. 파이프라인에 Element 추가
    if (!add_elements_to_pipeline()) {
        stop();
        return false;
    }

    // 6. 정적 구간 연결
    if (!link_static_elements()) {
        stop();
        return false;
    }

    // 7. tee branch 연결
    if (!link_tee_branches()) {
        stop();
        return false;
    }

    // 8. Dynamic Pad 콜백 등록
    g_signal_connect(
        source_,
        "pad-added",
        G_CALLBACK(&CameraPipeline::on_pad_added),
        this
    );

    return true;
}

bool CameraPipeline::create_elements() {
    // 1. Element 생성
    source_ = gst_element_factory_make("rtspsrc", "source");
    depay_ = gst_element_factory_make("rtph264depay", "depay");
    parser_ = gst_element_factory_make("h264parse", "parser");
    tee_ = gst_element_factory_make("tee", "tee");

    record_queue_ = gst_element_factory_make("queue", "record-queue");
    split_sink_ = gst_element_factory_make("splitmuxsink", "split-sink");

    fake_queue_ = gst_element_factory_make("queue", "fake-queue");
    fake_sink_ = gst_element_factory_make("fakesink", "fake-sink");

    // 2. Element 생성 확인
    if (source_ == nullptr || depay_ == nullptr || parser_ == nullptr ||
        tee_ == nullptr || record_queue_ == nullptr || split_sink_ == nullptr ||
        fake_queue_ == nullptr || fake_sink_ == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;
        return false;
    }

    return true;
}

bool CameraPipeline::configure_elements() {
    // 1. rtspsrc 설정
    g_object_set(
        source_,
        "location", rtsp_uri_.c_str(),
        "latency", 100,
        nullptr
    );

    // 2. h264parse 설정
    g_object_set(
        parser_,
        "config-interval", -1,
        nullptr
    );

    // 3. splitmuxsink 설정
    g_object_set(
        split_sink_,
        "location", record_pattern_.c_str(),
        "max-size-time", static_cast<guint64>(split_seconds_) * GST_SECOND,
        nullptr
    );

    // 4. fakesink 설정
    g_object_set(
        fake_sink_,
        "sync", FALSE,
        nullptr
    );

    return true;
}

bool CameraPipeline::add_elements_to_pipeline() {
    // 1. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline_),
        source_,
        depay_,
        parser_,
        tee_,
        record_queue_,
        split_sink_,
        fake_queue_,
        fake_sink_,
        nullptr
    );

    return true;
}

bool CameraPipeline::link_static_elements() {
    // 1. depay → parser → tee 연결
    if (!gst_element_link_many(depay_, parser_, tee_, nullptr)) {
        std::cerr << "Failed to link depay, parser, tee" << std::endl;
        return false;
    }

    // 2. record branch 내부 연결
    if (!gst_element_link(record_queue_, split_sink_)) {
        std::cerr << "Failed to link record_queue and split_sink" << std::endl;
        return false;
    }

    // 3. fake branch 내부 연결
    if (!gst_element_link(fake_queue_, fake_sink_)) {
        std::cerr << "Failed to link fake_queue and fake_sink" << std::endl;
        return false;
    }

    return true;
}

bool CameraPipeline::link_tee_branches() {
    // 1. tee request pad 생성
    tee_record_pad_ = gst_element_request_pad_simple(tee_, "src_%u");
    tee_fake_pad_ = gst_element_request_pad_simple(tee_, "src_%u");

    // 2. queue sink pad 가져오기
    record_queue_sink_pad_ = gst_element_get_static_pad(record_queue_, "sink");
    fake_queue_sink_pad_ = gst_element_get_static_pad(fake_queue_, "sink");

    if (tee_record_pad_ == nullptr || tee_fake_pad_ == nullptr ||
        record_queue_sink_pad_ == nullptr || fake_queue_sink_pad_ == nullptr) {
        std::cerr << "Failed to get tee or queue pads" << std::endl;
        return false;
    }

    // 3. tee → record queue 연결
    if (gst_pad_link(tee_record_pad_, record_queue_sink_pad_) != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link tee to record queue" << std::endl;
        return false;
    }

    // 4. tee → fake queue 연결
    if (gst_pad_link(tee_fake_pad_, fake_queue_sink_pad_) != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link tee to fake queue" << std::endl;
        return false;
    }

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
        source_ = nullptr;
        depay_ = nullptr;
        parser_ = nullptr;
        tee_ = nullptr;
        record_queue_ = nullptr;
        split_sink_ = nullptr;
        fake_queue_ = nullptr;
        fake_sink_ = nullptr;
        return;
    }

    // 2. 파이프라인 정지
    gst_element_set_state(pipeline_, GST_STATE_NULL);

    // 3. tee request pad 해제
    if (tee_ != nullptr && tee_record_pad_ != nullptr) {
        gst_element_release_request_pad(tee_, tee_record_pad_);
        gst_object_unref(tee_record_pad_);
        tee_record_pad_ = nullptr;
    }

    if (tee_ != nullptr && tee_fake_pad_ != nullptr) {
        gst_element_release_request_pad(tee_, tee_fake_pad_);
        gst_object_unref(tee_fake_pad_);
        tee_fake_pad_ = nullptr;
    }

    // 4. queue sink pad 참조 해제
    if (record_queue_sink_pad_ != nullptr) {
        gst_object_unref(record_queue_sink_pad_);
        record_queue_sink_pad_ = nullptr;
    }

    if (fake_queue_sink_pad_ != nullptr) {
        gst_object_unref(fake_queue_sink_pad_);
        fake_queue_sink_pad_ = nullptr;
    }

    // 5. 파이프라인 해제
    gst_object_unref(pipeline_);

    pipeline_ = nullptr;

    source_ = nullptr;
    depay_ = nullptr;
    parser_ = nullptr;
    tee_ = nullptr;

    record_queue_ = nullptr;
    split_sink_ = nullptr;

    fake_queue_ = nullptr;
    fake_sink_ = nullptr;
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