#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
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
		m_wndHorzSplit.Create ( *this, rcDefault );
		m_wndVertSplit.Create ( m_wndHorzSplit, rcDefault );
	
		//
		m_hWndClient = m_wndHorzSplit;
		//
		// Create the pane container.
		m_wndPaneContainer.Create ( m_wndVertSplit, TEXT("CPaneContainer Title"));

		//
		m_aboutDlg.Create(m_wndPaneContainer, rcDefault);
		m_wndPaneContainer.SetClient(m_aboutDlg);
		m_wndVertSplit.SetSplitterPane(SPLIT_PANE_LEFT, m_wndPaneContainer);
		//
		m_wndFormatList.Create(m_wndVertSplit, rcDefault);
		m_wndFormatList.Init();
		m_wndVertSplit.SetSplitterPane(SPLIT_PANE_RIGHT, m_wndFormatList);
		//
		//
	//	m_wndFormatList2.Create(m_wndHorzSplit, rcDefault);
	//	m_wndFormatList2.Init();
	//	m_wndHorzSplit.SetSplitterPane(SPLIT_PANE_TOP, m_wndFormatList2);
		//
		m_wndHorzSplit.SetSplitterPane(SPLIT_PANE_BOTTOM, m_wndVertSplit);
		//
		UpdateLayout();
		/*
			Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
			UpdateLayout() resizes the splitter window to its initial size. 
			If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
			The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
		*/
		m_wndVertSplit.m_cxyMin = 150;
		m_wndHorzSplit.m_cxyMin = 100;

		m_wndVertSplit.SetSplitterPos(200);
		m_wndHorzSplit.SetSplitterPos(200);
		
		//
		return 0;
	}
private:
	//
	CSplitterWindow m_wndVertSplit;
	CHorSplitterWindow m_wndHorzSplit;
	//CWindow
	CAboutDlg m_aboutDlg;
	CClipSpyListCtrl m_wndFormatList;
	CClipSpyListCtrl m_wndFormatList2;
	CPaneContainer m_wndPaneContainer;
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
