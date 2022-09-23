cmake_minimum_required(VERSION 3.20)
project(x)
set(CMAKE_CXX_STANDARD 11)

if(ANDROID)
    set(GTest_DIR "/home/zz/artifacts/googletest/master/android-arm64/lib/cmake/GTest")
endif()
find_package(GTest REQUIRED)

include(asan.cmake)

add_executable(testbed
    testbed.cpp
)

target_link_libraries(testbed
    GTest::gtest
    GTest::gtest_main
)