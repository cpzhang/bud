#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

//
int add_pointer(int* i)
{
	return ++(*i);
}
int add_reference(int& i)
{
	return ++i;
}
// Entry Point
int main()
{
	int i = 1;
	add_pointer(&i);
	add_reference(i);
}

#endif // __bootstrapper_cpp__
