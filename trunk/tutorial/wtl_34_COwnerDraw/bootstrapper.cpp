#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include "resource.h"

class CODButtonImpl : public CWindowImpl<CODButtonImpl, CButton>,
	public COwnerDraw<CODButtonImpl>
{
private:
	CBitmap m_bmp;
public:
	BEGIN_MSG_MAP(CODButtonImpl)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CODButtonImpl>, 1)
	END_MSG_MAP()

	CODButtonImpl()
	{
		m_bmp.LoadBitmap(IDB_BITMAP);
	}
	void DrawItem ( LPDRAWITEMSTRUCT lpdis )
	{
		// NOTE: m_bmp is a CBitmap init'ed in the constructor.
		CDCHandle dc = lpdis->hDC;
		CDC dcMem;

		dcMem.CreateCompatibleDC ( dc );
		dc.SaveDC();
		dcMem.SaveDC();

		// Draw the button's background, red if it has the focus, blue if not.
		if ( lpdis->itemState & ODS_FOCUS ) 
			dc.FillSolidRect ( &lpdis->rcItem, RGB(255,0,0) );
		else
			dc.FillSolidRect ( &lpdis->rcItem, RGB(0,0,255) );

		// Draw the bitmap in the top-left, or offset by 1 pixel if the button
		// is clicked.
		dcMem.SelectBitmap ( m_bmp );

		if ( lpdis->itemState & ODS_SELECTED ) 
			dc.BitBlt ( 1, 1, 80, 80, dcMem, 0, 0, SRCCOPY );
		else
			dc.BitBlt ( 0, 0, 80, 80, dcMem, 0, 0, SRCCOPY );

		dcMem.RestoreDC(-1);
		dc.RestoreDC(-1);
	}
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
		m_buttonBitMap.SubclassWindow(GetDlgItem(IDC_BitMap));
		//
		CenterWindow();
		return TRUE;
	}
private:
	CODButtonImpl m_buttonBitMap;
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
