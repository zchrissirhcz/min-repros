cmake_minimum_required(VERSION 3.20)
project(hello)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
add_executable(testbed
  main.cpp
)

target_link_libraries(testbed log)