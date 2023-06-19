cmake_minimum_required(VERSION 3.20)

project(use_hello)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(testbed testbed.cpp)
target_include_directories(testbed PUBLIC
  "${CMAKE_SOURCE_DIR}/../hello"
)
target_link_libraries(testbed PUBLIC
  "${CMAKE_SOURCE_DIR}/../hello/build/vs2022-x64/Release/hello.lib"
  #"${CMAKE_SOURCE_DIR}/../hello/build/vs2022-x64/Debug/hello.lib"
)
