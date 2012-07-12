#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

//
class Interface
{
public:
	void write()
	{
		std::cout<<"void Interface::write()"<<std::endl;
		_write();
		this->_write2();
	}
private:
	virtual void _write()
	{
		std::cout<<"void Interface::_write()"<<std::endl;
	}
	virtual void _write2()
	{
		std::cout<<"void Interface::_write2()"<<std::endl;
	}
};

class Generalize: public Interface
{
private:
	virtual void _write()
	{
		std::cout<<"void Generalize::_write()"<<std::endl;
	}
	virtual void _write2()
	{
		std::cout<<"void Generalize::_write2()"<<std::endl;
	}
};
// Entry Point
int main()
{
	//
	Interface* i = new Generalize;
	i->write();
	delete i;
	i = NULL;
}

#endif // __bootstrapper_cpp__
