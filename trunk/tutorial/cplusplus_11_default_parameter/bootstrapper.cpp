#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
#include <cstdlib>
#include <string>

//
void f(int a, int b = 0)
{

}

//
template<typename A, typename B = int>
class C
{
public:
	void f()
	{

	}
};

//标准模板类
template<typename T>
class D
{
public:
	D()
	{

	}
};
//特化模板，意义不大
template<>
class D<float>
{
public:
	D()
	{
		printf("D");
	}
};
//特化模板，指针
template<>
class D<float*>
{
public:
	D()
	{
		printf("D*");
	}
};
// Entry Point
int main()
{
	//
	f(0, 1);
	f(0);

	//
	C<float, int> g;
	g.f();
	C<float> h;
	h.f();

	//
	D<float> d;
	D<float> e;
	D<float*> i;
}

#endif // __bootstrapper_cpp__
