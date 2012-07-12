#include "mainFrame.h"
#include "aboutdlg.h"
#include "global.h"
#include "canvas.h"
#include <atlmisc.h>
#include <stack>
#include <cctype>
#include "IPrimitive.h"
extern CAppModule _Module;
Canvas* gCanvas;
//
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	static u32 oldTime = GetTickCount();
	u32 currentTim = GetTickCount();

	UIUpdateToolBar();

	//
	if (_canvas)
	{
		_canvas->update(currentTim - oldTime);
		_canvas->render();
	}
	oldTime = currentTim;
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

	_createVerticalSplitter();
	_createCanvas();
	_createHoricalSplitter();
	_createTreeView();
	_createAnimationBox();

	UpdateLayout();

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_RENDER_BAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIEnable(ID_EDIT_PASTE, FALSE);

	_canvas = NULL;
	//
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}
std::string standardFilePath(const std::string& path)
{
	std::string p = path;
	for (size_t i = 0; i != path.size(); ++i)
	{
		if (path[i] == '\\')
		{
			p[i] = '/';
		}
	}
	return p;
}
LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	CFileDialog fd(true, "entity", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Plato Documents (*.group;*.entity;*.mesh;*.mz;*.subentity)\0*.group;*.entity;*.mesh;*.mz;*.subentity\0All Files (*.*)\0*.*\0"));
	if (fd.DoModal() == IDOK)
	{
		std::string buf(512, 0);
		buf = fd.m_szFileName;
		if (!buf.empty() && _canvas)
		{
			_canvas->changeModelFile(standardFilePath(buf));
		}
	}
	return 0;
}
LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
#if 0
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
#endif

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
	//
	if (NULL == _canvas)
	{
		_canvas = new Canvas(m_view.m_hWnd);
		_canvas->addObserver(this);
		gCanvas = _canvas;
		_canvas->addObserver(&m_aniDlg);
	}

	return 0;
}

LRESULT CMainFrame::OnFileSave( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	// TODO: add code to initialize document
	CFileDialog fd(false, "entity", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Plato Documents (*.group;*.entity;*.mesh;*.mz;*.animation)\0*.group;*.entity;*.mesh;*.mz;*.animation\0All Files (*.*)\0*.*\0"));
	if (fd.DoModal() == IDOK)
	{
		std::string buf(512, 0);
		buf = fd.m_szFileName;
		if (!buf.empty() && _canvas)
		{
			_canvas->saveFile(buf);
		}
	}
	return 0;
}
LRESULT CMainFrame::OnTVSelChanged(int, LPNMHDR pnmh, BOOL&)
{
	// 如何解析参数
	LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pnmh);
	CTreeViewCtrlEx tree = pnmh->hwndFrom;
	if (tree.IsWindow())
	{
		CTreeItem item = tree.GetSelectedItem();
		std::string path;
		for (; !item.IsNull() && !item.GetParent().IsNull(); item = item.GetParent())
		{
			std::string name(256, 0);
			item.GetText((LPTSTR)name.c_str(), 256);
			std::string temp = name.c_str();
			if (!path.empty())
			{
				path = temp + "/" + path;
			}
			else
			{
				path = temp;
			}
		}
		//
		if (_canvas)
		{
			path = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + path;
			_canvas->changeModelFile(standardFilePath(path));
		}
	}

	return 0L;
}

void CMainFrame::_createVerticalSplitter()
{
	// create the vertical splitter
	m_vSplit.Create(*this, rcDefault/*, NULL, 0, WS_EX_CLIENTEDGE*/);
	m_hWndClient = m_vSplit;

	// set the vertical splitter parameters
	m_vSplit.m_cxyMin = 150; // minimum size
	//在设置分隔条的位置之前你需要设置 m_hWndClient 并调用 CFrameWindowImpl::UpdateLayout()
	UpdateLayout();
	m_vSplit.SetSplitterPos(150);
	//
	m_vSplit.SetSplitterExtendedStyle(SPLIT_PROPORTIONAL);
	//
	m_vSplit.m_bFullDrag = TRUE;
}

void CMainFrame::_createCanvas()
{
	m_vSplitPropertySheet.Create(m_vSplit, rcDefault);
	m_vSplit.SetSplitterPane(SPLIT_PANE_RIGHT, m_vSplitPropertySheet);
	//
	m_view.Create(m_vSplitPropertySheet, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_vSplitPropertySheet.SetSplitterPane(SPLIT_PANE_LEFT, m_view);
	//
	mProPageInformation.Create();
	mProSheet.AddPage(mProPageInformation);
	mProSheet.SetTitle("Property");
	HWND h = mProSheet.Create(m_vSplitPropertySheet);
	m_vSplitPropertySheet.SetSplitterPane(SPLIT_PANE_RIGHT, mProSheet);

	//int r = mProSheet.DoModal();
	mProSheet.SetActivePage(0);
	mProPageInformation.SetTitle("Information");
	//
	m_vSplitPropertySheet.m_cxyMin = 1000;
	UpdateLayout();
	m_vSplitPropertySheet.SetSplitterPos(1000);
}

void CMainFrame::_createHoricalSplitter()
{
	m_hSplit.Create(m_vSplit, rcDefault);

	m_vSplit.SetSplitterPane(SPLIT_PANE_LEFT, m_hSplit);
	// set the vertical splitter parameters
	m_hSplit.m_cxyMin = 250; // minimum size
	UpdateLayout();
	m_hSplit.SetSplitterPos(300);
}

void CMainFrame::_createTreeView()
{
	//
	m_tvCtrl.Create(m_hSplit, rcDefault, NULL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 
		WS_EX_CLIENTEDGE);
	m_hSplit.SetSplitterPane(SPLIT_PANE_TOP, m_tvCtrl);
	CTreeItem t;
	CTreeItem tail;
	//如何插入根节点
	{
		t = m_tvCtrl.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
		t.SetText("data");
	}
	//如何插入子节点
	{
		HANDLE hFind = INVALID_HANDLE_VALUE;
		std::string dataPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
		std::stack<std::string> pathes;
		std::stack<CTreeItem> items;
		pathes.push(dataPath);
		items.push(t);

		WIN32_FIND_DATA ffd;
		while(!pathes.empty())
		{
			t = items.top();
			items.pop();
			dataPath = pathes.top();
			pathes.pop();
			hFind = FindFirstFile((dataPath +  + "/*").c_str(), &ffd);
			do
			{
				std::string name = ffd.cFileName;
				if(name != "." && name != ".." )
				{
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						pathes.push(dataPath + "/" + ffd.cFileName);
						tail = t.AddTail(ffd.cFileName, 0);
						items.push(tail);
					}
					else
					{
						std::string name = Buddha::FileSystem::getInstancePtr()->getFileExtension(ffd.cFileName);
							std::transform(name.begin(), name.end(), name.begin(), std::tolower);
						if (name == "mz" || name == "mesh" || name == "entity" || name == "group" || name == "subentity")
						{
							t.AddTail(ffd.cFileName, 0);
						}
					}
				}
			}while (FindNextFile(hFind, &ffd) != 0);
		}
		
	}
	m_uPane.SetClient(m_tvCtrl);
}
void CMainFrame::_createAnimationBox()
{
	//
	m_dPane.Create(m_hSplit, "animation");
	m_hSplit.SetSplitterPane(SPLIT_PANE_BOTTOM, m_dPane);
	m_dPane.m_wndClient = m_aniDlg.Create(m_dPane.m_hWnd);
}

void CMainFrame::update( IPrimitive* p )
{
	m_vSplit.SetActivePane(SPLIT_PANE_RIGHT);
	m_animationBox.Clear();
	while(m_animationBox.GetCount())
	{
		m_animationBox.DeleteString(0);
	}

	AniMapIterator it = p->getAniMapIterator();
	while(!it.isAtEnd())
	{
		m_animationBox.AddString(it.getCurrentKey().c_str());
		++it;
	}
}

LRESULT CMainFrame::OnCBSelChanged( WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/ )
{
	// 如何解析参数
	int nSel = m_animationBox.GetCurSel();
	// Get the item text

	std::string strItemText(128, 0);
	m_animationBox.GetLBText(nSel, (char*)strItemText.c_str());
	_canvas->setAnimation(strItemText);
	return TRUE;
}

LRESULT CMainFrame::OnTrackBar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CString strPercent;
	int nPercent;
	bHandled = FALSE;

	switch ((int)LOWORD(wParam)) 
	{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			nPercent = (short)HIWORD(wParam);
			bHandled = TRUE;
			break;
		case SB_ENDSCROLL:
		case SB_LINELEFT:
		case SB_LINERIGHT:
		case SB_PAGELEFT:
		case SB_PAGERIGHT:
			nPercent = m_speedBar.GetPos();
			bHandled = TRUE;
			break;
		default:
			break;
	} // end switch
	return bHandled;
}

LRESULT CMainFrame::OnSize( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (_canvas)
	{
		//_canvas->onSize()
	}
	bHandled = FALSE;
	return 0L;
}
