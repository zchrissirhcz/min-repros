cmake_minimum_required(VERSION 3.20)

project(pop)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(testbed 
    testbed.cpp
    hello/hello.cpp
)

