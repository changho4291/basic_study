#ifndef __CAMERAPIPELINE_H__
#define __CAMERAPIPELINE_H__

#include <gst/gst.h>

#include <string>
#include <iostream>

class CameraPipeline {
private:
    std::string rtsp_uri_;

    GstElement* pipeline_ = nullptr;
    GstElement* depay_ = nullptr;

public:
    explicit CameraPipeline(std::string rtsp_uri);
    ~CameraPipeline();

    CameraPipeline(const CameraPipeline&) = delete;
    CameraPipeline& operator=(const CameraPipeline&) = delete;

    bool create();
    bool start();
    bool wait_until_error_or_eos();
    void stop();

private:
    static void on_pad_added (
        GstElement* src,
        GstPad* new_pad,
        gpointer user_data
    );

    void handle_pad_added(GstPad* new_pad);
};

#endif // __CAMERAPIPELINE_H__