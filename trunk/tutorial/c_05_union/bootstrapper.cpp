#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <stdio.h>
typedef union 
{
	char m[128];
	long data;
}value1;

typedef union 
{
	char m:3;
	char n:7;
	int data;
}value;
// Entry Point
int main()
{
	value1 v1;
	int i1 = sizeof(v1);
	value v;
	int i = sizeof(v);
	return 0;
}

#endif // __bootstrapper_cpp__
