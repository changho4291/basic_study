cmake_minimum_required(VERSION 3.20)

project(010_tee_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET 
    gstreamer-1.0
)

add_executable(gst_basic
    main.cpp
)

target_link_libraries(gst_basic
    PRIVATE
        PkgConfig::GSTREAMER
)