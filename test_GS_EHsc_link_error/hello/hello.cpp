#include "hello.h"
#include <stdio.h>

void hello(std::string& name)
{
    printf("hello, %s\n", name.c_str());
}

// ehsc.cpp
#include <iostream>

class NeedsDestruction {
public:
    NeedsDestruction () {
        std::cout << "Constructor called" << std::endl;
    }

    ~NeedsDestruction () {
        std::cout << "Destructor called" << std::endl;
    }
};

void my_throws () {
    NeedsDestruction nd;
    throw std::exception("exception");
}
