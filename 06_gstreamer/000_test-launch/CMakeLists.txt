cmake_minimum_required(VERSION 3.20)

project(000_test_launch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET
    gstreamer-1.0
    gstreamer-rtsp-server-1.0
)

add_executable(gst_rtsp_server
    test-launch.cpp
)

target_link_libraries(gst_rtsp_server
    PRIVATE
        PkgConfig::GSTREAMER
)