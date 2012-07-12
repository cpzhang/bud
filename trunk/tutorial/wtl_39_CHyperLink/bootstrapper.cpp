#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include "resource.h"

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
		m_wndLink.SetHyperLinkExtendedStyle(HLINK_USETAGS | HLINK_UNDERLINED );
		m_wndLink.SetHyperLink ( _T("http://www.google.com/") );

		if (!m_wndLink.SubclassWindow(GetDlgItem(IDC_STATIC)))
		{
			return FALSE;
		}
		//
		CenterWindow();
		return TRUE;
	}
private:
	CHyperLink  m_wndLink;
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
