# ghidra-android-arm-argument-order

This directory serves as minimal reproduces for Ghidra disassembly android armeabi-v7a / arm64-v8a platform that gets wrong argument order.

Ghidra version: 10.1.2 (ubuntu 20.04, x86_64)

## arm64 case
Reproduce steps:
1. Install android ndk-r21e
2. Build project with steps:
```
cd build
bash ./android-arm64-run.sh
```
3. Open Ghidra and import `build/android-arm64/CMakeFiles/testbed.dir/testbed.o`

4. The disassembly result will be the following:(argument order different with testbed.c)
```c++
void mcReluCInplace(float param_1,int param_2,int param_3,float *param_4)

{
  float fVar1;

  param_3 = param_3 * param_2;
  if (param_1 == 0.0) {
    if (0 < param_3) {
      fVar1 = *param_4;
      while( true ) {
        if (fVar1 < 0.0) {
          fVar1 = 0.0;
          *param_4 = 0.0;
        }
        if (param_3 == 1) break;
        param_3 = param_3 + -1;
      }
    }
  }
  else if (0 < param_3) {
    fVar1 = *param_4;
    do {
      if (fVar1 < 0.0) {
        fVar1 = fVar1 * param_1;
        *param_4 = fVar1;
      }
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  return;
}
```

## arm32 case
1. Install one of android ndk-r15c, ndk-16b, ndk-r19c, ndk-r22b
2. Build project with steps:
```
cd build
./android-arm32-build.sh
```
3.  Open Ghidra and import `build/android-arm32/CMakeFiles/testbed.dir/testbed.o`

4. Get disassembly result like the following (the if condition is wrong)
```c++
void mcReluCInplace(int width,int height,float slope,float *data)

{
  uint uVar1;
  float *in_r3;

  uVar1 = height * width & ~(height * width >> 0x1f);
  if ((float)data == 0.0) {  // !! This line is wrong. it should be  `if(slope == 0)`
    for (; uVar1 != 0; uVar1 = uVar1 - 1) {
      if ((int)((uint)(*in_r3 < 0.0) << 0x1f) < 0) {
        *in_r3 = 0.0;
      }
    }
  }
  else {
    for (; uVar1 != 0; uVar1 = uVar1 - 1) {
      if ((int)((uint)(*in_r3 < 0.0) << 0x1f) < 0) {
        *in_r3 = *in_r3 * (float)data;
      }
    }
  }
  return;
}
```
