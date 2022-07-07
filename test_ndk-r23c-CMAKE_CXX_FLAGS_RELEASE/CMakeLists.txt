cmake_minimum_required(VERSION 3.20)

project(inspect_ndk_cxx_flags_release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLASGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLASGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(testbed testbed.cpp)


