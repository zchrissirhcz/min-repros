cmake_minimum_required(VERSION 3.20)

project(x)

message("In Root CMakeLists.txt, CMAKE_SOURCE_DIR is: ${CMAKE_SOURCE_DIR}")
add_subdirectory(src)
