cmake_minimum_required(VERSION 3.20)
project(helloworld)

# This is the solution: let VS2019 >= 16.3 use FH3 instead of FH4
#include(msvc_exception_fh3.cmake)

add_library(hello STATIC
  hello.h
  hello.cpp
)

# Let's use the created static library, the `hello` target
add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test hello)