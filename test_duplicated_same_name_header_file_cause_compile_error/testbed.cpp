#include "inc/version.h"
#include "hello/version.h"

int main()
{
    Version ver;
    printf("%d.%d.%d\n", ver.major, ver.minor, ver.patch);
    return 0;
}

