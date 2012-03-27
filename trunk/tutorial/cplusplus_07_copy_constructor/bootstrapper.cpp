#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
class Book
{
private:
	int mPrice;
	char mName;
public:
	Book()
	{
		mPrice = 0;
		printf("Book()");
	}
	Book(const Book& b)
	{
		mPrice = b.mPrice;
		printf("Book(const Book& b)");
	}
	// 重载了赋值操作符，b3=a语句就调用此函数；否则，浅拷贝
	Book operator= (const Book& b)
	{
		mPrice = b.mPrice;
		mName = b.mName;
		return *this;
	}
};
// Entry Point
int main()
{
	Book b;
	Book b2(b);
	Book b3 = b;
	b3 = b2;
}

#endif // __bootstrapper_cpp__
