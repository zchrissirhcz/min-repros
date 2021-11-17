#!/bin/bash

BUILD_DIR=android-arm64
DST_DIR=/data/local/tmp

testbed()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=testbed

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb push ../tallgirl.jpg /data/local/tmp
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

testbed

