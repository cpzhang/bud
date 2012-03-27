#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <stdio.h>

int get()
{
	return 0;
}
// Entry Point
int main()
{
	int* address = (int*)get;
	__asm
	{
		call address;
	}
	return 0;
}

#endif // __bootstrapper_cpp__
