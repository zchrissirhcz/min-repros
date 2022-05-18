cmake_minimum_required(VERSION 3.15)
project(test_ncnn_warp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_executable(testbed simple.cpp)

set(ncnn_DIR "/home/zz/lib/ncnn/linux-x64/lib/cmake/ncnn")
find_package(ncnn)

if(ANDROID)
    set(OpenCV_DIR "/home/zz/artifacts/opencv/4.5.5/OpenCV-android-sdk/sdk/native/jni/abi-arm64-v8a")
else()
endif()
find_package(OpenCV REQUIRED)

add_executable(testbed
    testbed.cpp
    warpaffine.hpp
    warpaffine.cpp
)
target_link_libraries(testbed ${OpenCV_LIBS})

message(STATUS ">>> ANDROID_COMPILER_FLAGS_RELEASE: ${ANDROID_COMPILER_FLAGS_RELEASE}")
message(STATUS ">>> CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")