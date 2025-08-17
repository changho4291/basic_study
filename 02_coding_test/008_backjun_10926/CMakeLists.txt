cmake_minimum_required( VERSION 3.28.3 )

project(
	backjun_10926
    LANGUAGES C CXX
)

file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} 
	${SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)