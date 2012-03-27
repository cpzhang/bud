#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
#include <cstdlib>
#include <string>

template<class A, class B>
void f(A a, B b)
{

}

template<class A>
class C
{
public:
	void f()
	{

	}
};
// Entry Point
int main()
{
	int a, b;
	//
	f(a, b);

	float c,d;
	f(c, d);

	//
	C<int> e;
	e.f();
	//
	C<float> f;
	f.f();
}

#endif // __bootstrapper_cpp__
