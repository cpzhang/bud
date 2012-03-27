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
	// �����˸�ֵ��������b3=a���͵��ô˺���������ǳ����
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
