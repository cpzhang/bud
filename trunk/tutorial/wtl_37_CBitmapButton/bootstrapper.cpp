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
		if (!m_wndBmpBtn.SubclassWindow(GetDlgItem(IDC_BUTTON1)))
		{
			return FALSE;
		}
		
		// Set up the bitmap button
		CImageList iml;
		if (!iml.CreateFromImage ( IDB_BITMAP, 81, 1, CLR_NONE,	IMAGE_BITMAP, LR_CREATEDIBSECTION ))
		{
			return FALSE;
		}

		m_wndBmpBtn.SetToolTipText ( _T("Alyson") );
		m_wndBmpBtn.SetImageList ( iml );
		m_wndBmpBtn.SetImages ( 0, 1, 2, 3);
		//
		CenterWindow();
		return TRUE;
	}
private:
	CCheckListViewCtrl m_wndBmpBtn;
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
