cmake_minimum_required(VERSION 3.20)
project(x)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(asan.cmake)

add_executable(testbed testbed.cpp)

#--- GoogleTest
if(DEFINED ENV{ARTIFACTS_DIR})
  set(ARTIFACTS_DIR "$ENV{ARTIFACTS_DIR}")
else()
  message(WARNING "ARTIFACTS_DIR env var not defined, abort")
endif()

set(gtest_ver
    #1.11.0
    1.11.0-dev
)
if(ANDROID)
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/${gtest_ver}/android-arm64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/${gtest_ver}/linux-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/${gtest_ver}/windows/vs2019-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(GTest_DIR "${ARTIFACTS_DIR}/googletest/${gtest_ver}/mac-x64/lib/cmake/GTest" CACHE PATH "Directory that contains GTestConfig.cmake")
else()
  message(WARNING "GTest_DIR not set yet")
endif()
message(STATUS ">>> GTest_DIR is: ${GTest_DIR}")
find_package(GTest REQUIRED)

target_link_libraries(testbed GTest::gtest GTest::gtest_main)

if(ANDROID)
  target_link_libraries(testbed log)
endif()