cmake_minimum_required(VERSION 3.20)

project(my)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

add_executable(testbed testbed.cpp)