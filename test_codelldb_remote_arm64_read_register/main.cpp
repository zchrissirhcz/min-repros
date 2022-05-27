#include <stdio.h>

struct Test
{
    int i;
    Test()
    {
        i=23;
    }
    void hello(int a, int b, const char* name)
    {
        int c = a + b;
        c += i;

        for (int j = 0; j < c; j++)
        {
            if (name)
            {
                printf("hello, %s\n", name);
            }
            else
            {
                printf("hello, world\n");
            }
        }
    }
};
int main()
{
    Test obj1;
    const char* name = "ChrisZZ";
    int a = 1;
    int b = 2;
    obj1.hello(a, b, name);
    printf("done\n");

    return 0;
}