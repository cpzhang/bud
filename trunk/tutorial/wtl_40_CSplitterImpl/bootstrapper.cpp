#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlsplit.h>
#include "resource.h"
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

typedef CWinTraitsOR<LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER>
CListTraits;

class CClipSpyListCtrl :
	public CWindowImpl<CClipSpyListCtrl, CListViewCtrl, CListTraits>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

	BEGIN_MSG_MAP(CClipSpyListCtrl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	//...
	void CClipSpyListCtrl::Init()
	{
		SetExtendedListViewStyle ( LVS_EX_FULLROWSELECT );

		InsertColumn ( 0, TEXT(" Ù–‘"), LVCFMT_LEFT, 0, 0 );
		InsertColumn ( 1, TEXT("»°÷µ∑∂Œß"), LVCFMT_LEFT, 0, 1 );

		SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
		SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );

		//
		InsertItem(0, TEXT("a"));
		InsertItem(1, TEXT("c"));
		//
		SetItemText(0, 0, TEXT("A"));
		SetItemText(0, 1, TEXT("B"));
		//
		SetItemText(1, 0, TEXT("C"));
		SetItemText(1, 1, TEXT("D"));
	}
};

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
		//
		m_wndSplit.Create ( *this, rcDefault );
	
		//
		m_aboutDlg.Create(m_wndSplit, rcDefault);
		m_wndSplit.SetSplitterPane(SPLIT_PANE_LEFT, m_aboutDlg);
		//
		m_wndFormatList.Create(m_wndSplit, rcDefault);
		m_wndFormatList.Init();
		m_wndSplit.SetSplitterPane(SPLIT_PANE_RIGHT, m_wndFormatList);
		//
		m_hWndClient = m_wndSplit;
		UpdateLayout();
		/*
			Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
			UpdateLayout() resizes the splitter window to its initial size. 
			If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
			The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
		*/
		m_wndSplit.SetSplitterPos(300);
		//m_wndSplit.SetSinglePaneMode();
		//
		return 0;
	}
private:
	//
	CSplitterWindow m_wndSplit;
	//CHorSplitterWindow 
	//CWindow
	CAboutDlg m_aboutDlg;
	CClipSpyListCtrl m_wndFormatList;
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
