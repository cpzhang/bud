/*
1.CAboutDlg��DoModal()���ú�û����ǰ��ctrl tab���󣬿ɼ�������취����rc�ļ��IDD_ABOUTBOX����STYLE�У�����WS_VISIBLE
2.�����Ǹ�����Ľ���취�����ձ飬�������Ӵ���Ҳ��������ͬ�����⣬���յĽ���취����CView��OnPaint����ִ��bHandled = false���Ϳ�����
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