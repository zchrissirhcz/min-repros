#!/bin/bash

ANDROID_NDK=~/soft/android-ndk-r23
TOOLCHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

# echo "=== TOOLCHAIN is: $TOOLCHAIN"

BUILD_DIR=android-arm64-legacy-true
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_USE_LEGACY_TOOLCHAIN_FILE=True \
    ../..

#ninja
#cmake --build . --verbose
cmake --build .

cd ..

