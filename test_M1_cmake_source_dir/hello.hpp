#pragma once

#include <stdio.h>
#include <iostream>

static void hello(const char* name)
{
    if (name == nullptr) {
        printf("hello world\n");
    }
    else {
        printf("hello, %s\n", name);
    }
}
