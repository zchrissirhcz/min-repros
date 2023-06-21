cmake_minimum_required(VERSION 3.20)

project(q)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_subdirectory(hello)

add_library(hello STATIC IMPORTED GLOBAL)
#set(BUILDENV "vs2022-x64")
#set(BUILDENV "vs2019-x64")
set(hello_build_dir "${CMAKE_SOURCE_DIR}/../hello/build/vs2019-x64")
set_target_properties(hello PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../hello"
  IMPORTED_LOCATION_DEBUG "${hello_build_dir}/Debug/hello.lib"
  IMPORTED_LOCATION_RELEASE "${hello_build_dir}/Release/hello.lib"
  IMPORTED_LOCATION_MINSIZEREL "${hello_build_dir}/Release/hello.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${hello_build_dir}/Release/hello.lib"
  VERSION 0.0.1
)

add_executable(testbed testbed.cpp)
target_link_libraries(testbed PUBLIC
  hello
)