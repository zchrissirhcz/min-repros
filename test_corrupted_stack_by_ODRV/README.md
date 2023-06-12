# Corrupted Stack by ODRV Example

ODR: One Definition Rule
ODRV: One Definition Rule Violation

## Environment
Currently only tested with VS2022.

Without ASan.

Default VS project in Debug mode, which enabled `/RTC` (run time check).

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

## Debug Output
![](CorruptedStack.png)

## Explanation
The function `world()` use `MYRECT`, whose definition should be the 5-member one:
```cpp
// rect.h
struct MYRECT
{
	float x;
	float y;
	float width;
	float height;
	float angle;
	MYRECT()
	{
		printf("hello's MYRECT\n");
	}
	MYRECT(float x, float y, float width, float height, float angle) : x(x), y(y), width(width), height(height), angle(angle) {}
};
```

However, the debugger show us that it uses the 6-member one:
```cpp
// hello.cpp
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
```

The hello.cpp's `MYRECT` is with `sizeof(MYRECT)=6*sizeof(float)`, whisch is greater than rect.h's `sizeof(MYRECT)=5*sizeof(float)`, thus the callstack is corrupted.

## Solution
The better solution is encapsulate all your code in an unique namespace.

## Referenecs
- <https://stackoverflow.com/questions/10671956/same-class-name-in-different-c-files>