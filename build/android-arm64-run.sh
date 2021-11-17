#!/bin/bash

BUILD_DIR=android-arm64
DST_DIR=/data/local/tmp
#EXE_FILE=testbed
# yuv_rgb_test  area_resize_test rgb2gray_test
EXE_FILE=yuv_rgb_test
#LOCAL_DIR=/home/zz/work/arccv/aidepth_pp_dbg/assets
LOCAL_DIR=/home/zz/data

# adb shell "mkdir -p $DST_DIR"
# adb push $BUILD_DIR/$EXE_FILE $DST_DIR
# adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

#test_yuv420sp_to_rgb_opencv
# DEVICE_DIR=$DST_DIR/yuv2rgb
# SAVE_DIR=$LOCAL_DIR/android/yuv2rgb
# mkdir -p $SAVE_DIR
# adb pull $DEVICE_DIR/ASRDebug_0_7680x4320.rgb24               $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_opencv_simd_7680x4320.rgb24        $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_7680x4320.bgr24               $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_7680x4320.rgb24     $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_7680x4320.bgr24     $SAVE_DIR/

# verify opencv
# DEVICE_DIR=$DST_DIR/yuv2rgb
# SAVE_DIR=$LOCAL_DIR/android/yuv2rgb
# adb pull $DEVICE_DIR/roi_768x2.rgb24     $SAVE_DIR/

#test_yuv420sp_to_rgb_carotene
# SAVE_DIR=$LOCAL_DIR/android/yuv2rgb
# mkdir -p $SAVE_DIR
# adb pull $DEVICE_DIR/ASRDebug_0_catotene_7680x4320.rgb24  $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_carotene_7680x4320.rgb24  $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_carotene_7680x4320.bgr24  $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_carotene_7680x4320.bgr24  $SAVE_DIR/

#test_rgb_to_yuv420sp_opencv
# DEVICE_DIR=$DST_DIR/rgb2yuv
# SAVE_DIR=$LOCAL_DIR/android/rgb2yuv
# mkdir -p $SAVE_DIR
# adb pull $DEVICE_DIR/ASRDebug_0_save_7680x4320.NV21             $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_opencv_7680x4320.NV21             $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_save_7680x4320.NV12             $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_bgr24_save_7680x4320.NV21  $SAVE_DIR/
# adb pull $DEVICE_DIR/ASRDebug_0_from_bgr24_save_7680x4320.NV12  $SAVE_DIR/

# test for area_resize_test
# DEVICE_DIR=$DST_DIR/area_resize
# SAVE_DIR=$LOCAL_DIR/area_resize/android
# mkdir -p $SAVE_DIR

# adb pull $DEVICE_DIR/nv21_rgb_area_resize_divisible_save_2560x1440.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/nv21_rgb_area_resize_divisible_trial1_save_2560x1440.rgb24 $SAVE_DIR

# adb pull $DEVICE_DIR/opencv_cvtColor_resize_2560x1440.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/opencv_cvtColor_resize_2560x1440.png $SAVE_DIR

# adb pull $DEVICE_DIR/nv21_rgb_area_resize_divisible_save_2560x1440.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/nv21_rgb_area_resize_divisible_save_2560x1440.png $SAVE_DIR

# adb pull $DEVICE_DIR/opencv_cvtColor_resize_1097x617.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/opencv_cvtColor_resize_1097x617.png $SAVE_DIR

# adb pull $DEVICE_DIR/nv21_rgb_area_resize_non_divisible_reference_save_1097x617.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/nv21_rgb_area_resize_non_divisible_reference_save_1097x617.png $SAVE_DIR

# adb pull $DEVICE_DIR/nv21_rgb_area_resize_non_divisible_save_1097x617.rgb24 $SAVE_DIR
# adb pull $DEVICE_DIR/nv21_rgb_area_resize_non_divisible_save_1097x617.png $SAVE_DIR

# adb pull $DST_DIR/ASRDebug_0_save_7680x4320.NV21 /home/zz/work/arccv/aidepth_pp_dbg/assets/ASRDebug_0_save_7680x4320.NV21
# adb pull $DST_DIR/ASRDebug_0_save_7680x4320.NV12 /home/zz/work/arccv/aidepth_pp_dbg/assets/ASRDebug_0_save_7680x4320.NV12
# adb pull $DST_DIR/ASRDebug_0_from_bgr24_save_7680x4320.NV21 /home/zz/work/arccv/aidepth_pp_dbg/assets/ASRDebug_0_from_bgr24_save_7680x4320.NV21
# adb pull $DST_DIR/ASRDebug_0_from_bgr24_save_7680x4320.NV12 /home/zz/work/arccv/aidepth_pp_dbg/assets/ASRDebug_0_from_bgr24_save_7680x4320.NV12

#adb pull $DST_DIR/ASRDebug_0_opencv_7680x4320.NV21 /home/zz/work/arccv/aidepth_pp_dbg/assets/ASRDebug_0_opencv_7680x4320.NV21




# rgb2gray_test
rgb2gray_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=rgb2gray_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/rgb2gray
    SAVE_DIR=$LOCAL_DIR/rgb2gray/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DST_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    adb pull $DEVICE_DIR/rgb2gray_naive.png $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt1.png  $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt2.png  $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt3.png  $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt4.png  $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt5.png  $SAVE_DIR
    adb pull $DEVICE_DIR/rgb2gray_opt6.png  $SAVE_DIR
}

yuv_rgb_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=yuv_rgb_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/yuv2rgb
    SAVE_DIR=$LOCAL_DIR/yuv2rgb/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DST_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    #test_yuv420sp_to_rgb_carotene
    adb pull $DEVICE_DIR/ASRDebug_0_catotene_7680x4320.rgb24  $SAVE_DIR/
    adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_carotene_7680x4320.rgb24  $SAVE_DIR/
    adb pull $DEVICE_DIR/ASRDebug_0_carotene_7680x4320.bgr24  $SAVE_DIR/
    adb pull $DEVICE_DIR/ASRDebug_0_from_nv12_carotene_7680x4320.bgr24  $SAVE_DIR/

}


yuv420sp2rgb_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=yuv420sp2rgb_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/yuv420sp2rgb
    SAVE_DIR=$LOCAL_DIR/yuv420sp2rgb/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DEVICE_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    #test_yuv420sp_to_rgb_carotene
    # adb pull $DEVICE_DIR/yuv420sp2rgb_naive.png   $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opencv.png  $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opt1.png    $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opt2.png    $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opt3.png    $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opt4.png    $SAVE_DIR/
    # adb pull $DEVICE_DIR/yuv420sp2rgb_opt5.png    $SAVE_DIR/
    adb pull $DEVICE_DIR/yuv420sp2rgb_opt6.png    $SAVE_DIR/
    #adb pull $DEVICE_DIR/yuv420sp2rgb_opt7.png    $SAVE_DIR/
    #adb pull $DEVICE_DIR/yuv420sp2rgb_carotene.png    $SAVE_DIR/
    # adb pull $DEVICE_DIR/firstline_opencv.txt $SAVE_DIR/
    # adb pull $DEVICE_DIR/firstline_naive.txt $SAVE_DIR/
}

yuv422sp2bgra_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=yuv422sp2bgra_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/yuv422sp2bgra
    SAVE_DIR=$LOCAL_DIR/yuv422sp2bgra/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DEVICE_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    adb pull $DEVICE_DIR/result_opencv.png  $SAVE_DIR
    adb pull $DEVICE_DIR/yuv422sp2bgra_naive.png  $SAVE_DIR
    adb pull $DEVICE_DIR/yuv422sp2bgra_opt1.png  $SAVE_DIR
    adb pull $DEVICE_DIR/yuv422sp2bgra_opt2.png  $SAVE_DIR
}

parallel_for_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=parallel_for_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/parallel_for
    SAVE_DIR=$LOCAL_DIR/parallel_for/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DEVICE_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    #adb pull $DEVICE_DIR/result_opencv.png  $SAVE_DIR
}

resize_nn_test()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=resize_nn_test
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/resize_nn
    SAVE_DIR=$LOCAL_DIR/resize_nn/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DEVICE_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    adb pull $DEVICE_DIR/resize_nn_naive.png  $SAVE_DIR
    adb pull $DEVICE_DIR/resize_nn_opencv.png  $SAVE_DIR
    adb pull $DEVICE_DIR/opencv_analysis.png  $SAVE_DIR
    adb pull $DEVICE_DIR/resize_nn_opencv_standalone.png  $SAVE_DIR
    
}


test_color()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=test_color
    LOCAL_DIR=/home/zz/data

    DEVICE_DIR=$DST_DIR/color
    SAVE_DIR=$LOCAL_DIR/color/android
    mkdir -p $SAVE_DIR

    adb shell "mkdir -p $DEVICE_DIR"
    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

    adb pull $DEVICE_DIR/test_gray.png  $SAVE_DIR
    
}

#rgb2gray_test
#yuv_rgb_test
#yuv420sp2rgb_test
#parallel_for_test

#yuv422sp2bgra_test

#resize_nn_test

test_color

