# VS DLL library won't work for both debug and release types

这个例子用来说明： 在 Windows MSVC 编译器下， 动态库 hello 的 Debug 或 Release 版本的任意一个， 都能被 client 程序的 Debug 和 Release 两种模式链接， 但是由于 hello 的 API 使用了 `std::string` 这一 STL 容器， 而 MSVC 的 STL 实现在 Debug 和 Release 模式下是不一样的， 于是会导致 client 程序在 Debug 和 Release 模式下输出不一样， 并且出现了非预期的 crash。