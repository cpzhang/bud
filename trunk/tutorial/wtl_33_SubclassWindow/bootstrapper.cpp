#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include "resource.h"

class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
	BEGIN_MSG_MAP(CEditImpl)
		MESSAGE_HANDLER( WM_CONTEXTMENU, OnContextMenu)
	END_MSG_MAP()

	LRESULT OnContextMenu (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		MessageBox("Edit control handled WM_CONTEXTMENU");
		return TRUE;
	}
};

class CMainDlg : public CDialogImpl<CMainDlg>
	//,public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_FORMVIEW};

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnBnClickedOk)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
	END_MSG_MAP()

	//...
// 	BEGIN_DDX_MAP(CMainDlg)
// 		DDX_CONTROL(IDC_EDIT, m_wndEdit)
// 	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// First DDX call, hooks up variables to controls.
		//DoDataExchange(false);
		m_wndEdit.SubclassWindow(GetDlgItem(IDC_EDIT));
		//
		CenterWindow();
		return TRUE;
	}

protected:
	CEditImpl   m_wndEdit;
public:
	LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		::PostQuitMessage(0);
		::EndDialog(m_hWnd, 1);
		return 0;
	}
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