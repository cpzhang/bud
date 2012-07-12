/*
1.CAboutDlg在DoModal()调用后，没有置前，ctrl tab键后，可见；解决办法，在rc文件里，IDD_ABOUTBOX属性STYLE中，增加WS_VISIBLE
2.上面那个问题的解决办法不够普遍，其他的子窗口也会遇到相同的问题，最终的解决办法是在CView的OnPaint里面执行bHandled = false，就可以了
*/

/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "resource.h"
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