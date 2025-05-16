cmake_minimum_required(VERSION 3.28.3)

project(
    circular_linked_list
    LANGUAGES C CXX
)

file(GLOB SOURCES *.c)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}  PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)