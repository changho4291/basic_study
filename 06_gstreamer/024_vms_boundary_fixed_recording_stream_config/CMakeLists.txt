cmake_minimum_required(VERSION 3.20)

project(024_vms_boundary_fixed_recording_stream_config LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET 
    gstreamer-1.0
    gstreamer-app-1.0
)

add_executable(gst_basic
    main.cpp
    CameraPipeline.cpp
    CameraWorker.cpp
    CameraManager.cpp
    RecordingSegmentStore.cpp
    RecordingStreamConfig.cpp
    RecordingStreamSelector.cpp
)

target_include_directories(gst_basic PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(gst_basic
    PRIVATE
        PkgConfig::GSTREAMER
)
