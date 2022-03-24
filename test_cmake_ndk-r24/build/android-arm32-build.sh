#!/bin/bash

#ANDROID_NDK=~/soft/android-ndk-r23b
#ANDROID_NDK=~/soft/android-ndk-r24
ANDROID_NDK=~/soft/android-ndk-r25-beta2

TOOLCHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

echo "ANDROID_NDK is $ANDROID_NDK"
echo "TOOLCHAIN is: $TOOLCHAIN"

BUILD_DIR=android-arm32
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="armeabi-v7a" \
    -DANDROID_ARM_NEON=ON \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release \
    ../..

#ninja
#cmake --build . --verbose
cmake --build .

cd ..