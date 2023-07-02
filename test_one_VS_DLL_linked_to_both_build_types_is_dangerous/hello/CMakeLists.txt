cmake_minimum_required(VERSION 3.20)
project(hello)

set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(hello SHARED
    hello.cpp
)
