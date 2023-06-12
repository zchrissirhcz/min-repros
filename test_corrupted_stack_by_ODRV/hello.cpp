#include "hello.h"

#include <stdio.h>

struct MYRECT
{
	float left;
	float top;
	float right;
	float down;
	float cx;
	float cy;
	MYRECT()
	{
		left = 0, right = 0, top = 0, down = 0, cx = 0, cy = 0;
		printf("rect.h's MYRECT\n");
	}
	MYRECT(float left, float top, float right, float down) : left(left), top(top), right(right), down(down) { cx = (left + right) / 2; cy = (top + down) / 2; }
};


void hello(const char* name)
{
    printf("hello, %s\n", name);
    MYRECT r;
}