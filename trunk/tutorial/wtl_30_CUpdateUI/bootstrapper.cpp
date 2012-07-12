#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include "resource.h"
CAppModule _Module;

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
	,public CUpdateUI<CMainWindow>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("First WTL window"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolbar)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
		CHAIN_MSG_MAP(CUpdateUI<CMainWindow>)
	END_MSG_MAP()

	// ...
	BEGIN_UPDATE_UI_MAP(CMainWindow)
		UPDATE_ELEMENT(ID_FILE_NEW, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_OPEN, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()
public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		CreateSimpleToolBar();
		CreateSimpleStatusBar(IDR_MAINFRAME);
		//
		// Set the initial state of the Clock menu items:
		UIEnable( ID_FILE_NEW, false );
		UIEnable( ID_FILE_OPEN, true );
		UISetCheck(ID_VIEW_TOOLBAR, true);
		return 0;
	}
private:
	//
	LRESULT OnViewToolbar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
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

LRESULT CMainWindow::OnViewToolbar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���������������
	BOOL bVisible = !::IsWindowVisible(m_hWndToolBar);
	::ShowWindow(m_hWndToolBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}
