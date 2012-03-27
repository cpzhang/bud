// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlsplit.h>

#include "resource.h"
#include "view.h"

//
class CExplorerCombo : public CWindowImpl<CExplorerCombo, CComboBoxEx>
{
public:
	enum
	{
		m_cxGap = 2,
		m_cxMinDropWidth = 200
	};

	CComboBox m_cb;
	CToolBarCtrl m_tb;
	SIZE m_sizeTB;

	DECLARE_WND_SUPERCLASS(_T("WtlExplorer_ComboBox"), CComboBoxEx::GetWndClassName())

	BEGIN_MSG_MAP(CExplorerCombo)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// let the combo box initialize itself
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

		if(lRet != -1)
		{
			// adjust the drop-down width
			m_cb = GetComboCtrl();
			m_cb.SetDroppedWidth(m_cxMinDropWidth);

			// create a toolbar for the GO button
			m_tb = CFrameWindowImplBase<>::CreateSimpleToolBarCtrl(m_hWnd, IDT_GO1, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE | TBSTYLE_LIST);
			LPCTSTR lpszStrings = _T("Go\0");
			m_tb.AddStrings(lpszStrings);

			RECT rect;
			m_tb.GetItemRect(0, &rect);
			m_sizeTB.cx = rect.right;
			m_sizeTB.cy = rect.bottom;
		}

		return lRet;
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CDCHandle dc = (HDC)wParam;
		CWindow wndParent = GetParent();

		// Forward this to the parent window, rebar bands are transparent
		POINT pt = { 0, 0 };
		MapWindowPoints(wndParent, &pt, 1);
		dc.OffsetWindowOrg(pt.x, pt.y, &pt);
		LRESULT lRet = wndParent.SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);
		dc.SetWindowOrg(pt.x, pt.y);

		bHandled = (lRet != 0);
		return lRet;
	}

	LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if(m_tb.m_hWnd == NULL)
		{
			bHandled = FALSE;
			return 1;
		}

		// copy the WINDOWPOS struct and adjust for the GO button
		WINDOWPOS wp = *(LPWINDOWPOS)lParam;
		wp.cx -= m_sizeTB.cx + m_cxGap;
		LRESULT lRet = DefWindowProc(uMsg, wParam, (LPARAM)&wp);

		// paint below the GO button
		RECT rcGo = { wp.cx, 0, wp.cx + m_sizeTB.cx + m_cxGap, wp.cy };
		InvalidateRect(&rcGo);

		// center the GO button relative to the combo box
		RECT rcCombo;
		m_cb.GetWindowRect(&rcCombo);
		int y = (rcCombo.bottom - rcCombo.top - m_sizeTB.cy) / 2;
		//		if(y < 0)
		//			y = 0;

		// position the GO button on the right
		m_tb.SetWindowPos(NULL, wp.cx + m_cxGap, y, m_sizeTB.cx, m_sizeTB.cy, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

		return lRet;
	}
};

//
//
class CMyPaneContainer : public CPaneContainerImpl<CMyPaneContainer>
{
public:
	DECLARE_WND_CLASS_EX(_T("WtlExplorer_PaneContainer"), 0, -1)

	void DrawPaneTitle(CDCHandle dc)
	{
		RECT rect = { 0 };
		GetClientRect(&rect);

		if(IsVertical())
		{
			rect.right = rect.left + m_cxyHeader;
			dc.DrawEdge(&rect, EDGE_ETCHED, BF_LEFT | BF_TOP | BF_BOTTOM | BF_ADJUST);
			dc.FillRect(&rect, COLOR_3DFACE);
		}
		else
		{
			rect.bottom = rect.top + m_cxyHeader;
			// we don't want this edge
			//			dc.DrawEdge(&rect, EDGE_ETCHED, BF_LEFT | BF_TOP | BF_RIGHT | BF_ADJUST);
			dc.FillRect(&rect, COLOR_3DFACE);
			// draw title only for horizontal pane container
			dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			dc.SetBkMode(TRANSPARENT);
			HFONT hFontOld = dc.SelectFont(GetTitleFont());
			rect.left += m_cxyTextOffset;
			rect.right -= m_cxyTextOffset;
			if(m_tb.m_hWnd != NULL)
				rect.right -= m_cxToolBar;;
#ifndef _WIN32_WCE
			dc.DrawText(m_szTitle, -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
#else // CE specific
			dc.DrawText(m_szTitle, -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
#endif //_WIN32_WCE
			dc.SelectFont(hFontOld);
		}
	}
};

//
class Canvas;
//
class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, 
	public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CView m_view;

	CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		//MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCutOrCopy)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCutOrCopy)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		//订阅事件回调
		NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnTVSelChanged)
		//
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_PASTE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
	END_UPDATE_UI_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnEditCutOrCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void InitViews();
	HRESULT FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent);
	//
	LRESULT OnTVSelChanged(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
private:
	Canvas*	_canvas;
	CSplitterWindow m_wndSplitter;//分割窗口，条状
	CMyPaneContainer m_wndFolderTree;
	CTreeViewCtrlEx m_wndTreeView;
	CListViewCtrl m_wndListView;
	CExplorerCombo m_wndCombo;
};
