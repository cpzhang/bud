#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
#include <iostream>

class Apple
{
	int mWeight;
	char mType; 
	static int mName;
public:
	Apple()
		:mWeight(0)
	{
		mType = 1;
	}
	~Apple()
	{

	}
	int addWeight(int w)
	{
		mWeight += w;
		return mWeight;
	}
	static int getName()
	{
		return mName;
	}
};
int Apple::mName(2);
// Entry Point
int main()
{
	int f;
	int e(bool l);
	Apple c;
	Apple b();
	Apple a;
	int w = a.addWeight(1);
	w = a.getName();
}

#endif // __bootstrapper_cpp__
