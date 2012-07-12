#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include "resource.h"
// Custom styles for our check list ctrl. Note that when subclassing an existing
// control, only the list extended styles (LVS_EX_*) are used, so that's why
// the first two params here are 0.
typedef CCheckListViewCtrlImplTraits<0, 0, LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_UNDERLINEHOT | LVS_EX_ONECLICKACTIVATE> CMyCheckListTraits;

class CMyCheckListCtrl : 
	public CCheckListViewCtrlImpl<CMyCheckListCtrl, CListViewCtrl, CMyCheckListTraits>
{
private:
	typedef CCheckListViewCtrlImpl<CMyCheckListCtrl, CListViewCtrl, CMyCheckListTraits> baseClass;
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_CheckListView"), GetWndClassName())

	BEGIN_MSG_MAP(CMyCheckListCtrl)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_FORMVIEW};

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		// 
		REFLECT_NOTIFICATIONS()//转发给child处理
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		if (!m_wndChkList.SubclassWindow(GetDlgItem(IDC_LIST)))
		{
			return FALSE;
		}
		
		// Set up the check list ctrl.
		m_wndChkList.InsertColumn ( 0, _T("Scoobies"), LVCFMT_LEFT, 100, 0 );

		m_wndChkList.InsertItem ( 0, _T("Willow") );
		m_wndChkList.InsertItem ( 1, _T("Buffy") );
		m_wndChkList.InsertItem ( 2, _T("Giles") );

		m_wndChkList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );

		//
		CenterWindow();
		return TRUE;
	}
private:
	CMyCheckListCtrl m_wndChkList;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainDlg mw;
	mw.DoModal();
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
