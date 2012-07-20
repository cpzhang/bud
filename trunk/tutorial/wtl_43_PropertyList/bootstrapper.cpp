#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlframe.h>
#include <atlsplit.h>
#include <atldlgs.h>
#include "resource.h"
#include "PropertyList/PropertyList.h"
CAppModule _Module;
class CPropertyDlg : public CDialogImpl<CPropertyDlg>
{
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CPropertyDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_LIST1, PIN_BROWSE, OnBrowse)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		m_list.SubclassWindow(GetDlgItem(IDC_LIST1));

		 m_list.SetExtendedListStyle(PLS_EX_CATEGORIZED);
		 //
		 HPROPERTY hAppearance = m_list.AddItem( PropCreateCategory(_T("Appearance"), 1234) );
		 HPROPERTY hName = m_list.AddItem( PropCreateSimple(_T("Name"), _T("bjarke")) );
		 //
		 m_list.AddItem( PropCreateSimple(_T("X"), 123L) );
		 m_list.AddItem( PropCreateSimple(_T("Y"), 456L) );

		 HPROPERTY hBehaviour = m_list.AddItem( PropCreateCategory(_T("Behaviour")) );
		 m_list.AddItem( PropCreateSimple(_T("Enabled"), false) );
		 m_list.AddItem( PropCreateFileName(_T("Picture"), _T("C:\\Temp\\Test.bmp")));
		//
		 LPCTSTR list[] = { _T("0 - Red"), _T("1 - Blue"), _T("2 - Green"), _T("3 - Yellow"), NULL };
		 m_list.AddItem( PropCreateList(_T("Colors"), list, 2) );

		 // Testing control methods
		 HPROPERTY hFind = m_list.FindProperty(_T("Name")); hFind;
		 hFind = m_list.FindProperty(1234); hFind;
		 m_list.SetItemEnabled( hName, FALSE );
		 m_list.SetItemData(hName, 666);
		 LPARAM lParam = m_list.GetItemData(hName); lParam;
		 TCHAR szText[32];
		 m_list.GetItemName(hName, szText, 32);
		 CComVariant v(L"Form1");
		 m_list.SetItemValue(hName, &v);
		 m_list.CollapseItem(hAppearance);
		 m_list.ExpandItem(hAppearance);

		 //
		 m_list.CollapseItem(hBehaviour);
		 //
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
	LRESULT OnBrowse(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMPROPERTYITEM nmp = (LPNMPROPERTYITEM)pnmh;
		LPCTSTR lpcstrFilter = 
			_T("Bitmap Files (*.bmp)\0*.bmp\0")
			_T("All Files (*.*)\0*.*\0")
			_T("");
		CFileDialog dlg(TRUE, _T("bmp"), _T("C:\\Temp\\Image.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpcstrFilter);
		int nRet = dlg.DoModal();
		if(nRet == IDOK) {
			CComVariant v(dlg.m_ofn.lpstrFile);
			m_list.SetItemValue(nmp->prop, &v);
		}
		return 0;
	}
private:
	CPropertyListCtrl m_list;
};

typedef CWinTraitsOR<LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER>
CListTraits;

class CClipSpyListCtrl :
	public CWindowImpl<CClipSpyListCtrl, CListViewCtrl, CListTraits>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

	BEGIN_MSG_MAP(CClipSpyListCtrl)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	//...
	void CClipSpyListCtrl::Init()
	{
		SetExtendedListViewStyle ( LVS_EX_FULLROWSELECT );

		InsertColumn ( 0, TEXT(" Ù–‘"), LVCFMT_LEFT, 0, 0 );
		InsertColumn ( 1, TEXT("»°÷µ∑∂Œß"), LVCFMT_LEFT, 0, 1 );

		SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
		SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );

		//
		InsertItem(0, TEXT("a"));
		InsertItem(1, TEXT("c"));
		//
		SetItemText(0, 0, TEXT("A"));
		SetItemText(0, 1, TEXT("B"));
		//
		SetItemText(1, 0, TEXT("C"));
		SetItemText(1, 1, TEXT("D"));
	}
};

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("First WTL window"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
	END_MSG_MAP()

public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//
		m_wndHorzSplit.Create ( *this, rcDefault );
		m_wndVertSplit.Create ( m_wndHorzSplit, rcDefault );
	
		//
		m_hWndClient = m_wndHorzSplit;
		//
		// Create the pane container.
		m_wndPaneContainer.Create ( m_wndVertSplit, TEXT("CPaneContainer Title"));

		//
		m_aboutDlg.Create(m_wndPaneContainer, rcDefault);
		m_wndPaneContainer.SetClient(m_aboutDlg);
		m_wndVertSplit.SetSplitterPane(SPLIT_PANE_LEFT, m_wndPaneContainer);
		//
		m_wndFormatList.Create(m_wndVertSplit, rcDefault);
		m_wndFormatList.Init();
		m_wndVertSplit.SetSplitterPane(SPLIT_PANE_RIGHT, m_wndFormatList);
		//
		//
	//	m_wndFormatList2.Create(m_wndHorzSplit, rcDefault);
	//	m_wndFormatList2.Init();
	//	m_wndHorzSplit.SetSplitterPane(SPLIT_PANE_TOP, m_wndFormatList2);
		//
		m_wndHorzSplit.SetSplitterPane(SPLIT_PANE_BOTTOM, m_wndVertSplit);
		//
		UpdateLayout();
		/*
			Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
			UpdateLayout() resizes the splitter window to its initial size. 
			If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
			The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
		*/
		m_wndVertSplit.m_cxyMin = 150;
		m_wndHorzSplit.m_cxyMin = 100;

		m_wndVertSplit.SetSplitterPos(200);
		m_wndHorzSplit.SetSplitterPos(200);
		
		//
		return 0;
	}
private:
	//
	CSplitterWindow m_wndVertSplit;
	CHorSplitterWindow m_wndHorzSplit;
	//CWindow
	CPropertyDlg m_aboutDlg;
	CClipSpyListCtrl m_wndFormatList;
	CClipSpyListCtrl m_wndFormatList2;
	CPaneContainer m_wndPaneContainer;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	//_Module.Init(NULL, hInstance); 

	CMainWindow mw;
	mw.CreateEx();
	mw.ShowWindow(SW_MAXIMIZE);
	mw.UpdateWindow();
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
	//_Module.Term();
	return 0;
}
