#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

class Book
{
public:
	static int getPrice()
	{
		return tPrice;
	}
	static int tPrice;
};//1
int Book::tPrice(0);
// Entry Point
int main()
{
	Book b;
	size_t i = sizeof(b);
	i = Book::tPrice;
	i = Book::getPrice();
}

#endif // __bootstrapper_cpp__
