#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include "resource.h"
CAppModule _Module;

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("First WTL window"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
	END_MSG_MAP()

public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		CreateSimpleToolBar();
		return 0;
	}
private:
	//
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	//_Module.Init(NULL, hInstance);

	CMainWindow mw;
	mw.CreateEx();
	mw.ShowWindow(nCmdShow);
	mw.UpdateWindow();
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

		}
	}
	//_Module.Term();
	return 0;
}
