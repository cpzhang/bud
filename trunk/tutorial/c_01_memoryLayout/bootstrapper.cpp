#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <stdio.h>

// Entry Point
int main()
{
	int stack_a = 0;
	int stack_b = 1;
	int* heap_a = new int[1];
	char* heap_b = new char[2];
	*heap_a = stack_a;
}

#endif // __bootstrapper_cpp__
