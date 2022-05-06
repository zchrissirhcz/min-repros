## Intro
For the issue https://github.com/android/ndk/issues/1607

## Modify ndk-r23b to integrate with cmake correctly
```bash
export NDK_DIR=/home/zz/soft/android-ndk-r23b
cd $NDK_DIR/toolchains/llvm/prebuilt/linux-x86_64/bin
rm clang    # it's content is `clang-12`
rm clang++  # it's content is `clang-12`
mv clang-12 clang
ln -sf ./clang ./clang++ 
```

Otherwise you'll see cmake erros like this:
```
-- Android: Targeting API '21' with architecture 'arm64', ABI 'arm64-v8a', and processor 'aarch64'
-- Android: Selected unified Clang toolchain
-- The C compiler identification is unknown
-- The CXX compiler identification is unknown
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - failed
-- Check for working C compiler: /home/zz/soft/android-ndk-r24-beta1/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
-- Check for working C compiler: /home/zz/soft/android-ndk-r24-beta1/toolchains/llvm/prebuilt/linux-x86_64/bin/clang - broken
CMake Error at /home/zz/soft/cmake-3.22.0-rc3/share/cmake-3.22/Modules/CMakeTestCCompiler.cmake:69 (message):
  The C compiler

    "/home/zz/soft/android-ndk-r24-beta1/toolchains/llvm/prebuilt/linux-x86_64/bin/clang"

  is not able to compile a simple test program.

  It fails with the following output:

    Change Dir: /home/zz/work/test/test_ncnn_warp/build/android-arm64/CMakeFiles/CMakeTmp
    
    Run Build Command(s):/usr/bin/ninja cmTC_ee791 && [1/2] Building C object CMakeFiles/cmTC_ee791.dir/testCCompiler.c.o
    FAILED: CMakeFiles/cmTC_ee791.dir/testCCompiler.c.o 
    /home/zz/soft/android-ndk-r24-beta1/toolchains/llvm/prebuilt/linux-x86_64/bin/clang    -o CMakeFiles/cmTC_ee791.dir/testCCompiler.c.o -c /home/zz/work/test/test_ncnn_warp/build/android-arm64/CMakeFiles/CMakeTmp/testCCompiler.c
    clang-13: error: no input files
    ninja: build stopped: subcommand failed.
```

## Compile and run
```bash
git clone https://github.com/zchrissirhcz/test_ncnn_warp
cd test_ncnn_warp
cd build
# ensure cmake 3.22-rc3 is installed and in PATH
# modify `android-arm64-build.sh`'s android ndk directory, if necessary
./android-arm64-build.sh
# plug android phone to PC
./android-arm64-run.sh
```

## Performance test result

Using the provided code:
- testbed.cpp
- warpaffine.cpp
- warpaffine.hpp

src image: width=1920, height=1080
dst image: width=960, height=540

Together with OpenCV-4.5.4 (only for loading data, not affect the performance result), the performance result is:

| ndk version | time cost |
| -------- | --------- |
| ndk-r21b | 3.621770 ms |
| ndk-r22b | 3.475886 ms |
| ndk-r23b | 177.813490 ms |
| ndk-r24-beta1 | 3.544636 ms |