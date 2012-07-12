#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include "resource.h"
CAppModule _Module;
class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	//enum { IDD = IDD_ABOUT };
	enum { IDD = IDD_FORMVIEW1 };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDNO, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		mLB.Attach(::GetDlgItem(m_hWnd, IDC_LIST1));
		//
		mLB.AddString(TEXT("Hello ATL!"));
		mLB.AddString(TEXT("Hello WTL!"));
		mLB.AddString(TEXT("本来无一物，何处惹尘埃!"));
		//
		mLB.SetCurSel(0);

		CenterWindow();
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int n = mLB.GetCurSel();
		TCHAR b[256];
		memset(b, 0, 256);
		mLB.GetText(n, b);
		::MessageBox(NULL, b, TEXT("Caption"), MB_OK);
		EndDialog(wID);
		return 0;
	}

private:
	CListBox mLB;
};


class CMainWindow : 
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	BEGIN_MSG_MAP(CMainWindow)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, onOpen)
	END_MSG_MAP()

public:
	CMainWindow()
	{

	}
private:
	LRESULT onOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl,
		BOOL& bHandled)
	{
		CAboutDlg d;
		if (1)
		{
			if (d.DoModal() == IDOK)
			{
				bHandled = TRUE;
			}
		} 
		else
		{
			d.Create(NULL);
		}
		
		return 0;
	}
	void OnFinalMessage(HWND hWnd) 
	{
		PostQuitMessage(0);
	}
};


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	CMainWindow::GetWndClassInfo().m_wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	mw.Create(NULL);
	mw.CenterWindow();
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
	return 0;
}
