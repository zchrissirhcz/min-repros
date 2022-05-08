#!/bin/bash

#ANDROID_NDK=~/soft/android-ndk-r21e
#ANDROID_NDK=~/soft/android-ndk-r23b
#ANDROID_NDK=~/soft/android-ndk-r24-beta2
#ANDROID_NDK=~/soft/android-ndk-r25-beta2
ANDROID_NDK=~/soft/android-ndk-r25-beta3

TOOLCHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

BUILD_DIR=android-arm64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Debug \
    ../..

cmake --build .

cd ..
