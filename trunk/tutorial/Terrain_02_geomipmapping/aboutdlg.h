/*
1.CAboutDlg��DoModal()���ú�û����ǰ��ctrl tab���󣬿ɼ�������취����rc�ļ��IDD_ABOUTBOX����STYLE�У�����WS_VISIBLE
2.�����Ǹ�����Ľ���취�����ձ飬�������Ӵ���Ҳ��������ͬ�����⣬���յĽ���취����CView��OnPaint����ִ��bHandled = false���Ϳ�����
*/

/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "resource.h"
#include "atlbase.h"
#include "atlwin.h"
#include "atlapp.h"
#include "atlctrls.h"
#include <atlframe.h>
#include "IObserver.h"
#include "canvas.h"
#include "atlmisc.h"
class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};

class CAnimationDlg :
	public CDialogImpl<CAnimationDlg>,
	public CUpdateUI<CAnimationDlg>,
	public IObserver
{
private:
	CListBox mAnimationListBox;
	CTrackBarCtrl m_TrackBar;
	CButton mCycle;
	CButton mPlay;
	CButton mStop;
	bool mIsAnimated;
public:
	enum { IDD = IDD_FORMVIEW_ANIMATION };

	BEGIN_MSG_MAP(CAnimationDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_HSCROLL, OnNMThemeChangedSliderSpeed)
		COMMAND_ID_HANDLER(IDC_BUTTON_Play, OnAnimationPlay)
		COMMAND_ID_HANDLER(IDC_BUTTON_Stop, OnAnimationStop)
		COMMAND_ID_HANDLER(IDC_RADIO_Cycle, OnAnimationCycle)
		COMMAND_HANDLER(IDC_LIST_Animations, LBN_SELCHANGE, OnLbnSelchangeListAnimations)
		//
		CHAIN_MSG_MAP(CUpdateUI<CAnimationDlg>)
	END_MSG_MAP()

	//
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(IDC_BUTTON_Play, UPDUI_ENABLED)
		UPDATE_ELEMENT(IDC_BUTTON_Stop, UPDUI_ENABLED) 
		UPDATE_ELEMENT(IDC_RADIO_Cycle, UPDUI_CHECKED)
	END_UPDATE_UI_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		HWND hLB = GetDlgItem(IDC_LIST_Animations);
		mAnimationListBox = hLB;
		//
		m_TrackBar = GetDlgItem(IDC_SLIDER_Speed);
		m_TrackBar.SetRange(0, 100);
		m_TrackBar.SetPos(50);
		//
		mCycle = GetDlgItem(IDC_RADIO_Cycle);
		mCycle.SetCheck(1);
		//
		mIsAnimated = false;
		mPlay = GetDlgItem(IDC_BUTTON_Play);
		mPlay.EnableWindow(mIsAnimated);
		mStop = GetDlgItem(IDC_BUTTON_Stop);
		mStop.EnableWindow(!mIsAnimated);
		//
		CenterWindow(GetParent());
		return TRUE;
	}
	LRESULT OnAnimationPlay( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
	
	LRESULT OnAnimationStop( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
	
	LRESULT OnAnimationCycle( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
	
public:
	virtual void update(IPrimitive* p);
	
public:
	LRESULT OnLbnSelchangeListAnimations(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnNMThemeChangedSliderSpeed(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};


class CInformationDlg :
	public CDialogImpl<CInformationDlg>,
	public IObserver
{
private:
	CTabCtrl mInformation;
	CListBox mListBox;
	CListViewCtrl mListCtrl;
public:
	enum { IDD = IDD_FORMVIEW_Information };

	BEGIN_MSG_MAP(CInformationDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//
		NOTIFY_HANDLER(IDC_TAB_Information, TCN_SELCHANGE, OnTcnSelchangeTabInformation)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		mInformation = GetDlgItem(IDC_TAB_Information);
		mInformation.AddItem(TEXT("Information"));
		mInformation.AddItem(TEXT("Skin"));
		mInformation.AddItem(TEXT("Material"));
		//
		mListBox = GetDlgItem(IDC_LIST1);
		mListBox.AddString(TEXT("lb"));
		mListBox.AddString(TEXT("lb2"));

		//
		//mListCtrl = GetDlgItem(IDC_LIST2);
		//mListCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
		CRect	rectContent;
		GetClientRect(rectContent);
		mListCtrl.Create( m_hWnd, rectContent, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT, LVS_EX_FULLROWSELECT );
		mListCtrl.AddColumn( _T("Subject"), 0 );
		mListCtrl.AddColumn( _T("Due Date"), 1);
		mListCtrl.SetColumnWidth( 0, 300 );
		mListCtrl.SetColumnWidth( 1, 200 );
		mListCtrl.AddItem( 0, 0, TEXT("Finish WTL SnapSplitter class") );
		mListCtrl.AddItem( 0, 1, TEXT("Overdue") );
		//
		CenterWindow(GetParent());
		return TRUE;
	}

public:
	virtual void update(IPrimitive* p);
public:
	LRESULT OnTcnSelchangeTabInformation(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};