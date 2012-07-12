/************************************************************************/
/*
To hook up a CContainedWindow, you do four things:
1.Create a CContainedWindowT member variable in the dialog.
2.Put handlers in an ALT_MSG_MAP section of the dialog's message map
3.In the dialog's constructor, call the CContainedWindowT constructor and tell it which ALT_MSG_MAP section it should route messages to.
4.In OnInitDialog(), call the CContainedWindowT::SubclassWindow() method to associate a variable with a control.
*/
/************************************************************************/
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

		ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_CHAR, OnEditChar)
	END_MSG_MAP()


	CAboutDlg()
	:m_editSubClass(this, 1)
	{

	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		mLB.Attach(::GetDlgItem(m_hWnd, IDC_LIST1));
		//
		mLB.AddString(TEXT("Hello ATL!"));
		mLB.AddString(TEXT("Hello WTL!"));
		mLB.AddString(TEXT("������һ��δ��ǳ���!"));
		//
		mLB.SetCurSel(0);

		//
		// Create the contained window, routing its messages to us
		m_edit.Create("edit", this, 1, m_hWnd, &CWindow::rcDefault);
		m_edit.CenterWindow();

		//
		// Subclass the existing child edit control
		m_editSubClass.SubclassWindow(GetDlgItem(IDC_EDIT1));

		//
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

	// Let the child edit control receive only letters
	LRESULT OnEditChar(UINT, WPARAM wparam, LPARAM,	BOOL& bHandled) 
	{
			if( isalpha((TCHAR)wparam) ) 
				bHandled = FALSE;
			else      
				MessageBeep(0xFFFFFFFF);
			return 0;
	}


private:
	CListBox mLB;
	CContainedWindow m_edit;
	CContainedWindow m_editSubClass;
};


class CMainWindow : 
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, onOpen)
	END_MSG_MAP()

public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//����״̬�����ڡ� 
		m_hWndStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP, _T("Ready"), m_hWnd, 1);

		// Window class name provided by base class
		if( m_edit.Create(this, 1, m_hWnd, &CWindow::rcDefault) ) {
			// Now we can use methods of CEditT
			m_edit.InsertText( 0, _T("Here's some text") );
			return 0;
		}

		return 0;
	}

	LRESULT onOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl,
		BOOL& bHandled)
	{
		m_hWndStatusBar.SetWindowText(TEXT("open File���鲻��"));
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

private:
	//
	CContainedWindowT<CEdit> m_edit;
	CWindow		m_hWndStatusBar;
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
