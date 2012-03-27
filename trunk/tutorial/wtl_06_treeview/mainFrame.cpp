#include "mainFrame.h"
//
#include <atlstr.h>
//
#include "aboutdlg.h"
#include "global.h"
#include "canvas.h"

extern CAppModule _Module;
//
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();

	//
	if (_canvas)
	{
		_canvas->update();
		_canvas->render();
	}
	//
	static bool sFirst = true;
	if (sFirst)
	{
		sFirst = false;
		CComPtr<IShellFolder> spFolder;
		HRESULT hr = ::SHGetDesktopFolder(&spFolder);
		if(SUCCEEDED(hr))
		{
			CWaitCursor wait;

			FillTreeView(spFolder, NULL, TVI_ROOT);
			m_wndTreeView.Expand(m_wndTreeView.GetRootItem());
			m_wndTreeView.SelectItem(m_wndTreeView.GetRootItem());
		}
	}

	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Check if Common Controls 6.0 are used. If yes, use 32-bit (alpha) images
	// for the toolbar and command bar. If not, use the old, 4-bit images.
	UINT uResID = IDR_MAINFRAME_OLD;
	DWORD dwMajor = 0;
	DWORD dwMinor = 0;
	HRESULT hRet = AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
	if(SUCCEEDED(hRet) && dwMajor >= 6)
		uResID = IDR_MAINFRAME;

	// Set values to be displayed in the view window
	m_view.m_dwCommCtrlMajor = dwMajor;
	m_view.m_dwCommCtrlMinor = dwMinor;
	m_view.m_bAlpha = (uResID == IDR_MAINFRAME);

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(uResID);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, uResID, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	CreateSimpleStatusBar();

	//
	//m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	{

		m_hWndClient = m_wndSplitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		HWND hWndFolderTree = m_wndFolderTree.Create(m_wndSplitter, _T("Folders"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rc = { 50, 0, 300, 100 };
		m_wndCombo.Create(hWndFolderTree, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBS_DROPDOWN | CBS_AUTOHSCROLL);

		m_wndTreeView.Create(m_wndFolderTree, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
			TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 
			WS_EX_CLIENTEDGE);

		m_wndFolderTree.SetClient(m_wndTreeView);

		m_wndListView.Create(m_wndSplitter, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
			LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS,
			WS_EX_CLIENTEDGE);
		m_wndListView.SetExtendedListViewStyle(LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);

		InitViews();

		UpdateLayout();

		m_wndSplitter.SetSplitterPanes(m_wndFolderTree, m_wndListView);

		RECT rect;
		GetClientRect(&rect);
		m_wndSplitter.SetSplitterPos((rect.right - rect.left) / 4);
	}
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIEnable(ID_EDIT_PASTE, FALSE);

	//
	_canvas = NULL;
	//_canvas = new Canvas(m_hWndClient);
	//_canvas = new Canvas(m_hWndClient);
	
	//
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	CColorDialog cd;
	if (cd.DoModal() == IDOK)
	{
		COLORREF c = cd.GetColor();
		Euclid::Color e;
		e.setABGR(c);
		if (!RenderEngineImp::isNull())
		{
			RenderEngineImp::getInstancePtr()->setClearColor(e);
		}
	}

	//
	return 0;
}

LRESULT CMainFrame::OnEditCutOrCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Just to test enabled/disabled image state - toggle Paste button
	static BOOL bEnabled = FALSE;
	bEnabled = !bEnabled;
	UIEnable(ID_EDIT_PASTE, bEnabled);
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}
LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (_canvas)
	{
		_canvas->destroy();
		delete _canvas;
		_canvas = NULL;
	}
	bHandled = false;
	
	return 0;
}

LRESULT CMainFrame::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	
	return 0;
}

void CMainFrame::InitViews()
{
	//
	{
		CTreeItem t;
		CTreeItem tail;
		//如何插入根节点
		{
			t = m_wndTreeView.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
			t.SetText("root");

		}
		//如何插入子节点
		{
			tail = t.AddTail("child", 0);
		}
		//如同线性列表，head是头，tail是尾；如同std::list, push_front, push_back
		{
			tail.AddHead("head0", 0);
			tail.AddHead("head1", 0);
			tail.AddTail("tail0", 0);
			tail.AddTail("tail1", 0);
		}
	}
	
	// Create list view columns
	m_wndListView.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200, 0);
	m_wndListView.InsertColumn(1, _T("Size"), LVCFMT_RIGHT, 100, 1);
	m_wndListView.InsertColumn(2, _T("Type"), LVCFMT_LEFT, 100, 2);
}

HRESULT CMainFrame::FillTreeView(IShellFolder* pShellFolder, LPITEMIDLIST lpifq, HTREEITEM hParent)
{
	return E_POINTER;

}

LRESULT CMainFrame::OnTVSelChanged(int, LPNMHDR pnmh, BOOL&)
{
	// 如何解析参数
	{
		LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pnmh);
		CTreeViewCtrlEx tree = pnmh->hwndFrom;
		if (tree.IsWindow())
		{
			CTreeItem item = tree.GetSelectedItem();
			std::string name(256, 0);
			item.GetText((LPTSTR)name.c_str(), 256);

			// 如何插入列表
			int r = rand();
			m_wndListView.SetTextColor((DWORD)r);
			static int i = 0;
			if (0 == m_wndListView.GetItemCount())
			{
				m_wndListView.InsertItem(i, "ok");
			}
			m_wndListView.SetItemText(i, 0, name.c_str());
			m_wndListView.SetItemText(i, 1, "1");
			m_wndListView.SetItemText(i, 2, "2");
			//++i;
		}
	}
	return 0L;
}