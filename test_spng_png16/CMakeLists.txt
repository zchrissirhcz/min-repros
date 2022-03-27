cmake_minimum_required(VERSION 3.20)

project(x)

add_library(spng STATIC
    spng.h
    spng.c
)

add_executable(testbed testbed.cpp)

# --- opencv
if(ANDROID)
  set(OpenCV_DIR "/home/zz/artifacts/opencv/android-arm64/4.5.5/sdk/native/jni" CACHE PATH "" FORCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(OpenCV_DIR "/home/zz/artifacts/opencv/linux-x64/4.5.5/lib/cmake/opencv4")
endif()
message(STATUS ">>> OpenCV_DIR: ${OpenCV_DIR}")
find_package(OpenCV REQUIRED)


# --- libpng
if(ANDROID)
    set(libpng_install_dir "/home/zz/artifacts/libpng/1.6.38-dev/android-arm64")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(libpng_install_dir "/home/zz/artifacts/libpng/1.6.38-dev/linux-x64")
endif()

include("${libpng_install_dir}/lib/libpng/libpng16.cmake")
set_target_properties(png
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${libpng_install_dir}/include/libpng16
)
set_target_properties(png_static
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${libpng_install_dir}/include/libpng16
)

target_link_libraries(testbed 
    spng
    ${OpenCV_LIBS}
    png_static
)

# copy image files
add_custom_command(TARGET testbed
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/depth_image.png ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/1920x1080.png ${CMAKE_BINARY_DIR}/
)
