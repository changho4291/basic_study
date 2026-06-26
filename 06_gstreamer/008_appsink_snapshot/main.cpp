#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <fstream>
#include <iostream>
#include <string>

struct AppData {
    int frame_count = 0;
    bool saved = false;
    std::string output_file = "snapshot.ppm";
};

static bool save_rgb_as_ppm(
    const std::string& path,
    const guint8* data,
    int width,
    int height,
    size_t size
) {
    // 1. RGB 데이터 크기 확인
    const size_t expected_size = static_cast<size_t>(width) * height * 3;

    if (size < expected_size) {
        std::cerr << "Buffer is smaller than expected RGB frame size" << std::endl;
        return false;
    }

    // 2. PPM 파일 열기
    std::ofstream ofs(path, std::ios::binary);

    if (!ofs) {
        std::cerr << "Failed to open output file: " << path << std::endl;
        return false;
    }

    // 3. PPM 헤더 쓰기
    ofs << "P6\n"
        << width << " " << height << "\n"
        << "255\n";

    // 4. RGB 픽셀 데이터 쓰기
    ofs.write(
        reinterpret_cast<const char*>(data),
        static_cast<std::streamsize>(expected_size)
    );

    return ofs.good();
}

static GstFlowReturn on_new_sample(GstAppSink* appsink, gpointer user_data) {
    AppData* app = static_cast<AppData*>(user_data);

    // 1. appsink에서 sample 가져오기
    GstSample* sample = gst_app_sink_pull_sample(appsink);

    if (sample == nullptr) { return GST_FLOW_ERROR; }

    // 2. sample에서 caps / buffer 가져오기
    GstCaps* caps = gst_sample_get_caps(sample);
    GstBuffer* buffer = gst_sample_get_buffer(sample);

    if (caps == nullptr || buffer == nullptr) {
        gst_sample_unref(sample);
        return GST_FLOW_ERROR;
    }

    // 3. caps에서 영상 정보 가져오기
    GstStructure* structure = gst_caps_get_structure(caps, 0);

    gint width = 0;
    gint height = 0;

    const gchar* format = gst_structure_get_string(structure, "format");

    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    // 4. buffer 메모리 매핑
    GstMapInfo map_info;

    if (!gst_buffer_map(buffer, &map_info, GST_MAP_READ)) {
        gst_sample_unref(sample);
        return GST_FLOW_ERROR;
    }

    // 5. 프레임 카운트 증가
    app->frame_count++;

    std::cout << "Frame " << app->frame_count
              << " | format=" << (format ? format : "unknown")
              << " | width=" << width
              << " | height=" << height
              << " | size=" << map_info.size
              << " bytes"
              << std::endl;

    // 6. 첫 번째 프레임만 이미지로 저장
    if (!app->saved) {
        if (format != nullptr && g_strcmp0(format, "RGB") == 0) {
            bool ok = save_rgb_as_ppm(
                app->output_file,
                map_info.data,
                width,
                height,
                map_info.size
            );

            if (ok) {
                std::cout << "Saved snapshot: " << app->output_file << std::endl;
                app->saved = true;
            } else {
                std::cerr << "Failed to save snapshot" << std::endl;
            }
        } else {
            std::cerr << "Unsupported format for PPM: "
                      << (format ? format : "unknown")
                      << std::endl;
        }
    }

    // 7. buffer 메모리 매핑 해제
    gst_buffer_unmap(buffer, &map_info);

    // 8. sample 해제
    gst_sample_unref(sample);

    return GST_FLOW_OK;
}

int main(int argc, char* argv[]) {
    // 1. GStreamer 초기화
    gst_init(&argc, &argv);

    // 2. 저장 파일명 설정
    AppData app;

    if (argc >= 2) { app.output_file = argv[1]; }

    std::cout << "Output snapshot: " << app.output_file << std::endl;

    // 3. 파이프라인 생성
    GstElement* pipeline = gst_pipeline_new("appsink-snapshot-pipeline");

    // 4. Element 생성
    GstElement* source = gst_element_factory_make("videotestsrc", "source");
    GstElement* convert = gst_element_factory_make("videoconvert", "convert");
    GstElement* capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
    GstElement* sink = gst_element_factory_make("appsink", "sink");

    // 5. Element 생성 확인
    if (pipeline == nullptr || source == nullptr || convert == nullptr ||
        capsfilter == nullptr || sink == nullptr) {
        std::cerr << "Failed to create one or more GStreamer elements" << std::endl;

        if (pipeline != nullptr) {
            gst_object_unref(pipeline);
        }

        return 1;
    }

    // 6. source 속성 설정
    g_object_set(
        source,
        "num-buffers", 30,
        nullptr
    );

    // 7. capsfilter 속성 설정
    GstCaps* caps = gst_caps_from_string(
        "video/x-raw,format=RGB,width=640,height=360,framerate=30/1"
    );

    g_object_set(
        capsfilter,
        "caps", caps,
        nullptr
    );

    gst_caps_unref(caps);

    // 8. appsink 속성 설정
    g_object_set(
        sink,
        "emit-signals", TRUE,
        "sync", FALSE,
        nullptr
    );

    // 9. appsink 콜백 등록
    g_signal_connect(
        sink,
        "new-sample",
        G_CALLBACK(on_new_sample),
        &app
    );

    // 10. 파이프라인에 Element 추가
    gst_bin_add_many(
        GST_BIN(pipeline),
        source,
        convert,
        capsfilter,
        sink,
        nullptr
    );

    // 11. Element 연결
    if (!gst_element_link_many(source, convert, capsfilter, sink, nullptr)) {
        std::cerr << "Failed to link elements" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 12. 파이프라인 실행
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // 13. 실행 상태 확인
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to set pipeline to PLAYING" << std::endl;
        gst_object_unref(pipeline);
        return 1;
    }

    // 14. Bus 가져오기
    GstBus* bus = gst_element_get_bus(pipeline);

    // 15. ERROR 또는 EOS 메시지 대기
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    // 16. 메시지 처리
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

    // 17. Bus 해제
    gst_object_unref(bus);

    // 18. 파이프라인 정지
    gst_element_set_state(pipeline, GST_STATE_NULL);

    // 19. 파이프라인 해제
    gst_object_unref(pipeline);

    return 0;
}