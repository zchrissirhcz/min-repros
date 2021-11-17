cmake_minimum_required(VERSION 3.15)
project(test_ncnn_warp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_executable(testbed simple.cpp)

set(ncnn_DIR "/home/zz/lib/ncnn/linux-x64/lib/cmake/ncnn")
find_package(ncnn)

if(ANDROID)
    set(OpenCV_DIR "/home/zz/artifacts/opencv/android-arm64/4.5.4/sdk/native/jni")
else()
endif()
find_package(OpenCV REQUIRED)

add_executable(testbed
    testbed.cpp
    warpaffine.hpp
    warpaffine.cpp
)
target_link_libraries(testbed ${OpenCV_LIBS})