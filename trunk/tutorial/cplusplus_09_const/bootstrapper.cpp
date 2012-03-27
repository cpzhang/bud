#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
#include <cstdlib>
class Book
{
public:
	Book()
		:mPrice(0)
	{
	}
	int getPrice()
	{
		return mPrice;
	}
	int getPrice() const
	{
		return mPrice;
	}
private:
	const int mPrice;
	int mWeight;
};
const int i = 0;
// Entry Point
int main()
{
	//
	const int* ia = &i;
	ia = 0;
	//
	Book b;
	int a = b.getPrice();
	const int c = b.getPrice();
	//
	const Book e;
	int f = e.getPrice();
	const int g = e.getPrice();
}

#endif // __bootstrapper_cpp__
