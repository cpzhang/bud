#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <stdio.h>

//
int f(int a, int b = 0)
{
	return a + b;
}
// Entry Point
int main()
{
	int r = f(0, 1);
	r = f(2);
	return r;
}

#endif // __bootstrapper_cpp__
