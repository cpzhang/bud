#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
#include <cstdlib>
class Book
{
private:
	char* mName;
public:
	Book(char* i)
	{
		mName = i;
	}
	~Book(){}
#ifdef OPNew
	void* operator new(size_t s)
	{
		return malloc(s);
	}
	void operator delete(void* p)
	{
		free(p);
		p = 0;
	}
#endif
};
// Entry Point
int main()
{
	Book* b = new Book(0);
	delete b;
	b = 0;
}

#endif // __bootstrapper_cpp__
