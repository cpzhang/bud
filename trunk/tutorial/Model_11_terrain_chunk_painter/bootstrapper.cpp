#include "CMainWindow.h"
//���붨�����ȫ�ֱ���������Ƿ�
CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.CreateEx();
	//�������SW_MAXIMIZE��PIX������������
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