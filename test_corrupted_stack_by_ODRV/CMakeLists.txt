cmake_minimum_required(VERSION 3.20)
project(x)

add_library(hello STATIC
    hello.cpp
    hello.h
)

add_library(rect STATIC
    rect.cpp
    rect.h
)

add_executable(testbed
    main.cpp
)
target_link_libraries(testbed PUBLIC
    hello
    rect
)