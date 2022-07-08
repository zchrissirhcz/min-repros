# ndk-r23c cause Release build missing optimization flags

## The table
| ndk revision | CMAKE_CXX_FLAGS_RELEASE | CMAKE_CXX_FLAGS_RELEASE + LEGACY_TOOLCHAIN=True | CMAKE_CXX_FLAGS_RELEASE + LEGACY_TOOLCHAIN=False |
| ------------ | ----------------------- | ------- | -------- |
| ndk-r21e     | -O2    |   N/A |  N/A |
| ndk-r23      | -O3    |  -O2  | -O3  |
| ndk-r23b     | -O3    |   -   | -O3  | 
| ndk-r23c     |  -     |   -   | -O3  |

For ndk-r23c, if passing `-DCMAKE_BUILD_TYPE=Release`, the expected behaviour is, `CMAKE_CXX_FLAGS_RELEASE` is with `-O3` or `-O2`.

## Details
However, the following call, will get no `-O`s in the output:
```bash
cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release \
    ../..
```

```
-- CMAKE_BUILD_TYPE: Release
-- CMAKE_C_FLASGS: -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security   -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=pointer-arith -fno-common -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- CMAKE_CXX_FLASGS: -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security    -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- CMAKE_CXX_FLAGS_RELEASE: -DNDEBUG   -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- Configuring done
-- Generating done
-- Build files have been written to: /home/zz/work/test/dump_flags/build-r23c/android-arm64
ninja: no work to do.
```
(**And the missing `-O3` will cause poor performance for Release type build**)

If we explicitly add one option, `-DANDROID_USE_LEGACY_TOOLCHAIN_FILE=False`, then we'll get expected `-O3` in the output:
```bash
cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-21 \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_USE_LEGACY_TOOLCHAIN_FILE=False \
    ../..
```

```
-- CMAKE_BUILD_TYPE: Release
-- CMAKE_C_FLASGS: -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security -fexceptions  -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=pointer-arith -fno-common -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- CMAKE_CXX_FLASGS: -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security -fexceptions -frtti -stdlib=libc++  -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- CMAKE_CXX_FLAGS_RELEASE: -O3 -DNDEBUG  -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-stack-address -Werror=uninitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -Werror=multichar -Werror=parentheses
-- Configuring done
-- Generating done
-- Build files have been written to: /home/zz/work/test/dump_flags/build-r23c/android-arm64-legacy-false
ninja: no work to do.
```

Related: https://github.com/tencent/ncnn/pull/3976/files