#ifndef __bootstrapper_cpp__
#define __bootstrapper_cpp__

// Entry Point
int main()
{
	int i;
	unsigned int ui;
	{
		i = -1;
		ui = (unsigned int)i;
	}
	//
	{
		i = 1;
		ui = (int)i;
	}
}

#endif // __bootstrapper_cpp__
