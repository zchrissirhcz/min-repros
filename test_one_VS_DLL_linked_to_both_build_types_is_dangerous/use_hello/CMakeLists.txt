cmake_minimum_required(VERSION 3.20)

project(use_hello)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(copy_dll.cmake)

add_subdirectory(hello_shared_debug)
# add_subdirectory(hello_shared_release)
# add_subdirectory(hello_shared_debug_and_release)

add_executable(testbed testbed.cpp)
target_link_libraries(testbed PUBLIC
  hello
)

copy_target_dll(hello ${CMAKE_BINARY_DIR})