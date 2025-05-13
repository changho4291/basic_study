cmake_minimum_required( VERSION 3.28.3 )

project(
	generic_queue
    LANGUAGES C CXX
)

file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} 
	${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)