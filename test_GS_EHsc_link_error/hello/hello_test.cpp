#include "hello.h"
#include <stddef.h> // for NULL
#include <string>

int main()
{
    std::string s = "world";
    hello(s);
    hello(s);

    return 0;
}