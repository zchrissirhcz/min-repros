#!/bin/bash

#ANDROID_NDK=~/soft/android-ndk-r21b
ANDROID_NDK=~/soft/android-ndk-r21e
#ANDROID_NDK=~/soft/android-ndk-r23b
TOOLCHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

BUILD_DIR=android-arm64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_LD=lld \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Debug \
    -DARCPKG_VERBOSE=OFF \
    ../..

#ninja
#cmake --build . --verbose
cmake --build .  # &&  cmake --build . --target install  && cmake --build . --target arcpkg-export

cd ..
