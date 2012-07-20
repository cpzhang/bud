#include "CMainWindow.h"
//必须定义这个全局变量，否则非法
CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.CreateEx();
	//如果这里SW_MAXIMIZE，PIX不能正常工作
	mw.ShowWindow(SW_MAXIMIZE);
	//mw.ShowWindow(nCmdShow);
	mw.UpdateWindow();
	//mw.ShowWindow(SW_MAXIMIZE);
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	for (;;)
	{
		::ZeroMemory(&m, sizeof(m));
		if (::PeekMessage(&m, NULL, 0, 0, PM_NOREMOVE))
		{
			::GetMessage(&m, NULL, 0, 0);
			if (m.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&m);
			::DispatchMessage(&m);
		}
		else
		{
			mw.onIdle();
		}
	}

	return 0;
}