#pragma once
#include "atlHeaders.h"
#include "EventArgs.h"
#include "Chunk.h"
#include "SubEntity.h"
#include "Entity.h"
#include "FreeImage/Dist/FreeImage.h"
#include <stack>
#include <string>
#include <cctype>
extern Chunks      gTerrain;
class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BUTTON_OK, BN_CLICKED, OnBnClickedButtonOk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
public:
	LRESULT OnBnClickedButtonOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		EndDialog(0);
		return 0;
	}
};
class CGeometryDlg : public CDialogImpl<CGeometryDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_GEOMETRY};

	BEGIN_MSG_MAP(CGeometryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};
class CMaterialDlg : public CDialogImpl<CMaterialDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_MATERIAL};

	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};
class CEntityDlg : public CDialogImpl<CEntityDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_Entity};

	BEGIN_MSG_MAP(CEntityDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};
class CSubEntityDlg : public CDialogImpl<CSubEntityDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_SubEntity};

	BEGIN_MSG_MAP(CSubEntityDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_HANDLER(IDC_EDIT_Name, EN_CHANGE, OnEnChangeEditName)
	END_MSG_MAP()

	CSubEntityDlg()
	{
	}
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return TRUE;
	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		EventManager::getInstancePtr()->subscribeEvent(eEvent_CTreeDlgChangeFile, SubscriberSlot(&CSubEntityDlg::updateName, this));
		//
		mEditName = GetDlgItem(IDC_EDIT_Name);
		//
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
	LRESULT CSubEntityDlg::OnEnChangeEditName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
		// 发送该通知，除非重写 CDialogImpl<CSubEntityDlg>::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码

		return 0;
	}
	 bool updateName(const EventArgs& e)
	 {
		 EventArgsCTreeDlgChangeFile args = (EventArgsCTreeDlgChangeFile&)e;
		 std::string name("unknown");
		 switch (args.mType)
		 {
		 case eVisibleObjectType_SubEntity:
			 {
				 SubEntity* e = (SubEntity*)args.mPrimitive;
				 name = e->mName;
			 };
		 	break;
		 case eVisibleObjectType_Entity:
			 {
				 Entity* e = (Entity*)args.mPrimitive;
				 name = e->mName;
			 };
			 break;
		 }
		 mEditName.SetWindowText(name.c_str());
		 return true;
	 }
public:
	CEdit mEditName;
};

class CTreeDlg : public CDialogImpl<CTreeDlg>
{
private:
	CTreeViewCtrlEx m_tvCtrl;
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CTreeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		FORWARD_NOTIFICATIONS()// 把树状文件点击事件传给父窗口
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		//
		m_tvCtrl = GetDlgItem(IDC_TREE);
		//
		{
			CTreeItem t;
			CTreeItem tail;
			
			//如何插入子节点
			{
				HANDLE hFind = INVALID_HANDLE_VALUE;
				std::string dataPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
				//如何插入根节点
				{
					t = m_tvCtrl.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
					t.SetText(dataPath.c_str());
				}
				std::stack<std::string> pathes;
				std::stack<CTreeItem> items;
				pathes.push(dataPath);
				items.push(t);
				
				WIN32_FIND_DATA ffd;
				while(!pathes.empty())
				{
					t = items.top();
					items.pop();
					dataPath = pathes.top();
					pathes.pop();
					hFind = FindFirstFile((dataPath +  + "/*").c_str(), &ffd);
					do
					{
						std::string name = ffd.cFileName;
						if(name != "." && name != ".." )
						{
							if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								pathes.push(dataPath + "/" + ffd.cFileName);
								tail = t.AddTail(ffd.cFileName, 0);
								items.push(tail);
							}
							else
							{
								//if (name.find(".entity") != std::string::npos)
								{
									t.AddTail(ffd.cFileName, 0);
								}
							}
						}
					}while (FindNextFile(hFind, &ffd) != 0);
				}
				
			}
		}
		return TRUE;
	}
};

class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
public:
	BEGIN_MSG_MAP(CEditImpl)
	END_MSG_MAP()
};

class CCreateTerrainDlg : public CDialogImpl<CCreateTerrainDlg>
	,public CWinDataExchange<CCreateTerrainDlg>

{
private:
	//
	CEditImpl   mEditWidth;
	int			mWidth;
	//
	CEditImpl   mEditHeight;
	int			mHeight;
	//
	CEditImpl   mEditLOD;
	int			mLOD;
private:
	void clear()
	{
		mWidth = 1;
		mHeight = 1;
		mLOD = 0;
	}
public:
	CCreateTerrainDlg()
	{

	}
	~CCreateTerrainDlg()
	{
		clear();
	}
	enum { IDD = IDD_CreateTerrain};

	BEGIN_MSG_MAP(CCreateTerrainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CCreateTerrainDlg)
		DDX_CONTROL(IDC_EDIT_Width, mEditWidth)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_Width, mWidth, 1, 200)//做校验
		DDX_CONTROL(IDC_EDIT_Width, mEditHeight)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_Height, mHeight, 1, 200)//做校验
		DDX_CONTROL(IDC_EDIT_LOD, mEditLOD)//功能等价于SubclassWindow，接管消息回调
		DDX_INT_RANGE(IDC_EDIT_LOD, mLOD, 0, 6)//做校验
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		clear();
		// First DDX call, hooks up variables to controls.
		DoDataExchange(false); 
		//
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
public:
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		// Transfer data from the controls to member variables.
		if (DoDataExchange(true))
		{
			//
			EndDialog(0);
			//
			gTerrain.destroy();
			gTerrain.create(mWidth, mHeight, mLOD);
		}
		return 0;
	}

	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		EndDialog(0);
		return 0;
	}
	void OnDataValidateError ( UINT nCtrlID, BOOL bSave, _XData& data )
	{
		MessageBox("width and height range [1, 200], lod range [0, 6]");
		return;
	}

};


// A button that has an owner draw handler.
class CPainterTextureButton : public CWindowImpl<CPainterTextureButton, CButton>
{
public:
	BEGIN_MSG_MAP(CPainterTextureButton)
		MESSAGE_HANDLER( WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER( WM_LBUTTONUP, OnLeftButtonUp)
	END_MSG_MAP()
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
public:
	bool selected(bool b, bool forced = false);
private:
	void _clear()
	{
		mTextureName.clear();
		mID = eChunkTextureLayer_Size;
		mSelected = false;
	}
public:
	std::string mTextureName;
	eChunkTextureLayer mID;
	bool mSelected;
};