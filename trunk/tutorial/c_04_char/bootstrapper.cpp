#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <stdio.h>

// Entry Point
int main()
{
	char a = 0;
	char b = '0';
	char c[] = {"012"};
	char d[] = "012";
	char* e = "012";
	char* f = new char[2];
	delete f;
	f = 0;

	//
	char value1[] = {"hello"};
	char value2[] = {"hello"};
	char* pValue1 = "hello";
	char* pValue2 = "hello";
	//value1和value2地址一致吗？pValue1和pValue2呢？
	return 0;
}

#endif // __bootstrapper_cpp__
