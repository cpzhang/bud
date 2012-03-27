#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

class Book
{

};//1
class Book2
{
public:
	Book2()
	{

	}
	~Book2()
	{

	}
};//1
//
class Book3
{
	int price;
};//4
//
class Book4
{
public:
	virtual ~Book4(){}
};//4
//
class Book5
{
	int price;
public:
	virtual ~Book5(){}
};//8
//
class Book6: public Book
{

};//1
//
class Book7: public Book3
{

};//4
//
class Book8: public Book4
{

};//4
//
class Book9: public Book4
{
public:
	virtual ~Book9(){}
};//4
//
class Book10: public Book5
{

};//8

// Entry Point
int main()
{
	Book b;
	size_t i = sizeof(b);
	//
	Book2 b2;
	i = sizeof(b2);
	//
	Book3 b3;
	i = sizeof(b3);
	//
	Book4 b4;
	i = sizeof(b4);
	//
	Book5 b5;
	i = sizeof(b5);
	//
	Book6 b6;
	i = sizeof(b6);
	//
	Book7 b7;
	i = sizeof(b7);
	//
	Book8 b8;
	i = sizeof(b8);
	//
	Book9 b9;
	i = sizeof(b9);
	//
	Book10 b10;
	i = sizeof(b10);
}

#endif // __bootstrapper_cpp__
