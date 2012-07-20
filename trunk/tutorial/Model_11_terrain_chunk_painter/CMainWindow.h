#pragma once
#include "atlHeaders.h"
#include "CPropertyDlg.h"
#include "IdleHandler.h"
#include "CViewWindow.h"
#include "CAboutDlg.h"

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("ModelFrameWindowClass"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)
		NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnTVSelChanged) 
		COMMAND_ID_HANDLER(ID_EDIT_CREATETERRAIN, OnEditCreateterrain)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CMainWindow();
		LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) ;
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) ;
	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTVSelChanged(int, LPNMHDR pnmh, BOOL&);
	LRESULT OnEditCreateterrain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	CViewWindow mViewWindow;	
	CTreeDlg mTreeDlg;
	CPropertyDlg mPropertyDlg;
	CSplitterWindow m_wndSplitVertical;
	CSplitterWindow m_wndSplitVerticalProperty;
	CMultiPaneStatusBarCtrl m_wndStatusBar;
};
