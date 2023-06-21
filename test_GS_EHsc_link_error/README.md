# Visual Studio C++ Exception compatibility

## Problem description
Only consider Visual Studio >= 2015 versions.

For a lower version of VS(say, vs2017), we link an library (say, `hello`) which is build with higher version of VS(say, vs2019). The library code consists of C++ exception. Then our current project that using lower version of VS, will get link error. Such as symbol `__CxxFrameHandler4` not resolved.

## Problem description (Chinese)
（只考虑 VS >= 2015 的版本）

低版本的VS， 链接了由高版本 VS 编译的库（例如叫hello），并且hello的代码中用到了 Exception。

那么链接会报错， 报告的符号和 `/GS` 以及 `/EHSC` 有关。注意， CMake 生成的 VS 工程， 默认带有 `/GS` 和 `/EHsc` （在 CMAKE_CXX_FLAGS）.

## Reproduce Steps

Go to `hello` directory, run `build/vs2019-x64.cmd`

Go to `use_hello` directory, run `build/vs2017-x64.cmd`

## Error Output
```
hello.lib(hello.obj) : error LNK2001: 无法解析的外部符号 __CxxFrameHandler4 [D:\github\min-repros\test_GS_EHsc_link_error\use_he
llo\build\vs2017-x64\testbed.vcxproj]
D:\github\min-repros\test_GS_EHsc_link_error\use_hello\build\vs2017-x64\Debug\testbed.exe : fatal error LNK1120: 1 个无
法解
析的外部命令 [D:\github\min-repros\test_GS_EHsc_link_error\use_hello\build\vs2017-x64\testbed.vcxproj]
```

## References
- [/GS (Buffer Security Check)](https://learn.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=msvc-170)
- [/EH (Exception handling model)](https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170)