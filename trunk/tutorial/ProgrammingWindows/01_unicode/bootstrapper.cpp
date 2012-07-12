#include <windows.h>

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	{
		char c = 'A';
		char * p = "Hello!";
		char a[] = "Hello!";
	}
	{
		wchar_t c = 'A' ;
		wchar_t c2 = L'A' ;
		wchar_t * p = L"Hello!" ;
		static wchar_t a[] = L"Hello!" ;
	}
	{
		TCHAR c = 'A';
	}
	return 0;
}