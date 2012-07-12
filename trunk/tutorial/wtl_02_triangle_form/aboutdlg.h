/*
1.CAboutDlg在DoModal()调用后，没有置前，ctrl tab键后，可见；解决办法，在rc文件里，IDD_ABOUTBOX属性STYLE中，增加WS_VISIBLE
*/

/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "resource.h"
#include "atlbase.h"
#include "atlwin.h"
#include "global.h"
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


class CFormDlg : public CDialogImpl<CFormDlg>
{
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CFormDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		HWND hCanvas = GetDlgItem(IDC_CANVAS);
		new RenderEngineImp(hCanvas);
		CenterWindow(GetParent());
		return TRUE;
	}
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
private:
	void _renderGeometry();
	void _clear();
	bool _create();
	bool _isInitialized();
	//
private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
};