#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

class IPerson
{
public:
	int name;
	virtual int getName()
	{
		return name;
	}
};
class Man: public IPerson
{
public:
	virtual int getName()
	{
		return name;
	}
};
// Entry Point
int main()
{
	IPerson p;
	p.name = 0;
	int i = p.getName();
	//
	Man m;
	m.name = 1;
	i = m.getName();
	//
	Man* p2m = (Man*)&p;
	i = p2m->getName();
	//
	IPerson* m2p = &m;
	i = m2p->getName();
}

#endif // __bootstrapper_cpp__
