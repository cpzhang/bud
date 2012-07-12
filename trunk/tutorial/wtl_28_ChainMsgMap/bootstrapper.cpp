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
		mLB.AddString(TEXT("本来无一物，何处惹尘埃!"));
		//
		mLB.SetCurSel(0);

		//
		// Create the contained window, routing its messages to us
// 		m_edit.Create("edit", this, 1, m_hWnd, &CWindow::rcDefault);
// 		m_edit.CenterWindow();

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

template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd
{
public:
	CPaintBkgnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
	~CPaintBkgnd() { DeleteObject ( m_hbrBkgnd ); }

	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		HDC  dc = (HDC) wParam;
		RECT rcClient;

		pT->GetClientRect ( &rcClient );
		FillRect ( dc, &rcClient, m_hbrBkgnd );
		return 1;    // we painted the background
	}

protected:
	HBRUSH m_hbrBkgnd;
};

class CMainWindow : 
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
	,public CPaintBkgnd<CMainWindow, RGB(0,0,255)>
{
public:
	typedef CPaintBkgnd<CMainWindow, RGB(0,0,255)> CPaintBkgndBase;
	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, onOpen)

		CHAIN_MSG_MAP(CPaintBkgndBase)
	END_MSG_MAP()

public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//创建状态条窗口。 
		m_hWndStatusBar = ::CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP, _T("Ready"), m_hWnd, 1);

		// Window class name provided by base class
		if( m_edit.Create(this, 1, m_hWnd, &CWindow::rcDefault) ) {
			// Now we can use methods of CEditT
			m_edit.InsertText( 0, _T("Here's some text") );
			return 0;
		}

		return 0;
	}

	LRESULT onOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl,BOOL& bHandled)
	{
		m_hWndStatusBar.SetWindowText(TEXT("open File灵灵不昧"));
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
	CMainWindow::GetWndClassInfo().m_wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
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
