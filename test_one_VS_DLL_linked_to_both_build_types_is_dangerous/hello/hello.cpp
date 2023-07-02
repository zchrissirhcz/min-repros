#include "hello.h"

#include <stdio.h>
#include <iostream>
#include <string>

void hello(std::string& name)
{
    std::string s = "hello, ";
    if (name.length() < 5)
    {
        std::cout << "less name";
    }
    else
    {
        std::cout << "much name";
    }
    std::cout << " hello, " << name << "\n";
}