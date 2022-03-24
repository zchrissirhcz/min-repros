#!/bin/bash

#ANDROID_NDK=~/soft/android-ndk-r23b  # success
ANDROID_NDK=~/soft/android-ndk-r24  # fail during cmake

TOOLCHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

BUILD_DIR=android-arm64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-31 \
    -DCMAKE_BUILD_TYPE=Release \
    ../..

cmake --build .
cd ..
