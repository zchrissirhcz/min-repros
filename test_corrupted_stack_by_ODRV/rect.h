#include <stdio.h>


struct MYRECT
{
    float x;
    float y;
    float width;
    float height;
    float angle;
    MYRECT()
    {
        printf("MYRECT::MYRECT() in %s\n", __FILE__);
    }
    MYRECT(float x, float y, float width, float height, float angle) : x(x), y(y), width(width), height(height), angle(angle) {}
};



//#ifdef __cplusplus
//extern "C" {
//#endif

void world();

//inline void hello(const char* name)
//{
//    printf("This is rect.cpp's hello.\n");
//    printf("And, hello, %s\n", name);
//}

//#ifdef __cplusplus
//}
//#endif
