#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlsplit.h>
#include <atlddx.h>
#include "resource.h"
//必须定义这个全局变量，否则非法
CAppModule _Module;

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	//enum { IDD = IDD_ABOUT };
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("ModelFrameWindowClass"), IDR_MAINFRAME);

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
		//
		CreateSimpleToolBar();
		//
		return 0;
	}
private:
	
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.CreateEx();
	mw.ShowWindow(nCmdShow | SW_MAXIMIZE);
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
	return 0;
}
