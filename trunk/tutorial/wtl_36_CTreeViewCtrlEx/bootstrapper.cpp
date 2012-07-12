#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include "resource.h"

class CBuffyTreeCtrl : public CWindowImpl<CBuffyTreeCtrl, CTreeViewCtrlEx>,
	public CCustomDraw<CBuffyTreeCtrl>
{
private:
	CBitmap m_bmp;
public:
	BEGIN_MSG_MAP(CBuffyTreeCtrl)
		CHAIN_MSG_MAP_ALT(CCustomDraw<CBuffyTreeCtrl>, 1)
	END_MSG_MAP()
	DWORD CBuffyTreeCtrl::OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD)
	{
		return CDRF_NOTIFYITEMDRAW;
	}

	DWORD CBuffyTreeCtrl::OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD)
	{
		NMTVCUSTOMDRAW* pnmtv = (NMTVCUSTOMDRAW*) lpNMCD;
		switch ( lpNMCD->lItemlParam )
		{
		case 1:
			pnmtv->clrText = RGB(0,128,0);
			break;
			
		case 2:
			pnmtv->clrText = RGB(255,0,0);
			break;
			
		case 3:
			pnmtv->clrText = RGB(0,0,255);
			break;
			
		case 4:
			pnmtv->clrText = RGB(196,196,196);
			break;
		}
		 return CDRF_DODEFAULT;
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
		m_tc.SubclassWindow(GetDlgItem(IDC_TREE));
		//
		{
			CTreeItem t;
			CTreeItem tail;
			//如何插入根节点
			{
				t = m_tc.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
				t.SetText("root");
				m_tc.SetItemData(t, 1);
			}
			//如何插入子节点
			{
				tail = t.AddTail("child", 0);
				m_tc.SetItemData(tail, 2);
			}
			//如同线性列表，head是头，tail是尾；如同std::list, push_front, push_back
			{
				tail.AddHead("head0", 0);
				tail.AddHead("head1", 0);
				tail.AddTail("tail0", 0);
				tail.AddTail("tail1", 0);
			}
		}

		//
		CenterWindow();
		return TRUE;
	}
private:
	CBuffyTreeCtrl m_tc;
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
