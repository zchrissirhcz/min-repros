#pragma once

#include <string>

#define MY_EXPORTS

#ifdef MY_EXPORTS
#define MY_API __declspec(dllexport)
#else
#define MY_API __declspec(dllimport)
#endif

MY_API void hello(std::string& name);

