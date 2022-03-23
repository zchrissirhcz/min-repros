## purpose
For the issue of https://github.com/llvm/llvm-project/issues/53241

This minimal-reproducible-example was original in the repository with URL https://github.com/zchrissirhcz/tsan_test . Now it is merge into this new repo.

## Envs
ubuntu 20.04
clang-llvm latest
```
clang version 14.0.0 (/home/zz/work/github/llvm-project/clang cc35161dc716ab5b0e66b15ac320ebd451a2bc44)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/zz/soft/llvm14-dev/bin

```

build clang with `build/linux-x64.sh`:
```
#!/bin/bash

# note: this will very time costing
# you may drop some items in LLVM_ENABLE_PROJECTS list
# clang-tools-extra: contains many tools, such as clangd and clang-tidy

#-D LLVM_ENABLE_PROJECTS='clang;libcxx;libcxxabi;libunwind;lldb;compiler-rt;lld;polly;openmp;clang-tools-extra'

BUILD_DIR=linux-x64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake ../.. \
    -G Ninja \
    -D LLVM_ENABLE_PROJECTS='clang;lldb;compiler-rt;lld;polly' \
    -D LLVM_ENABLE_RUNTIMES=libcxx \
    -D LLVM_ENABLE_RUNTIMES=libcxxabi \
    -D LLVM_ENABLE_RUNTIMES=libunwind \
    -D CMAKE_BUILD_TYPE=Release \
    -D LLVM_TARGETS_TO_BUILD='X86' \
    -D CMAKE_INSTALL_LIBDIR='lib' \
    -D CMAKE_INSTALL_PREFIX=~/soft/llvm14-dev \
    -D CMAKE_C_COMPILER=clang \
    -D CMAKE_CXX_COMPILER=clang++ \
    ../../llvm
```
