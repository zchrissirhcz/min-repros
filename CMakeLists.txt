cmake_minimum_required(VERSION 3.20)
project(test_ncnn_warp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ncnn_DIR "/home/zz/lib/ncnn/linux-x64/lib/cmake/ncnn")
find_package(ncnn)

find_package(OpenCV REQUIRED)

add_executable(testbed
    testbed.cpp
    warpaffine.hpp
    warpaffine.cpp
)
target_link_libraries(testbed ${OpenCV_LIBS})