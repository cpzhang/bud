#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlsplit.h>
#include <atlddx.h>
#include "resource.h"
#include "renderImp.h"
#include <stack>
// idle handler
class IdleHandler
{
#define InterfaceIdleHandler(terminal)\
	public:\
	virtual void onIdle() ##terminal

#define IdleHandler_Base InterfaceIdleHandler(=0;)
#define IdleHandler_Derived InterfaceIdleHandler(;)
	IdleHandler_Base
};
//必须定义这个全局变量，否则非法
CAppModule _Module;
//
class CViewWindow: 
	public CWindowImpl<CViewWindow>
	,public IdleHandler
{
public:
	BEGIN_MSG_MAP(CViewWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_SIZE, onSize)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CViewWindow()
	{
		_clear();
	}
public:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		new RenderEngineImp(m_hWnd);
		//
		return 1;
	}
	LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) 
	{
		b = FALSE;
		if (!RenderEngineImp::getInstancePtr()->isInitialized())
		{
			RenderEngineImp::getInstancePtr()->create();
			_create();
		}

		//
		return 1;
	}
	LRESULT onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) 
	{
		b = FALSE;
		if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
		{
			return 1;
		}
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onInvalidateDevice();
		//
		Euclid::sViewPort vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = LOWORD(lParam);
		vp.Height = HIWORD(lParam);
		vp.MinZ = 0;
		vp.MaxZ = 1;
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setViewport(&vp);
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice(); 
		//
		return 1;
	}
	LRESULT onDestroy(UINT, WPARAM, LPARAM, BOOL&) 
	{
		if (RenderEngineImp::getInstancePtr()->isInitialized())
		{
			if (_material)
			{
				_material->destroy();
				delete _material;
				_material = 0;
			}
			//
			if (_vb)
			{
				_vb->destroy();
				delete _vb;
				_vb = NULL;
			}

			RenderEngineImp::getInstancePtr()->destroy();
			delete RenderEngineImp::getInstancePtr();
		}

		//
		return 1;
	}
	void _create()
	{
		//
		_material = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		_material->setEffect("shader\\Position.fx");
		//
		Euclid::sPosition vertices[3];
		vertices[0].position = Vec3(-1.0f, 0.0f, 0.0f);
		vertices[1].position = Vec3(0.0f, 1.0f, 0.0f);
		vertices[2].position = Vec3(1.0f, 0.0f, 0.0f);
		_vb = RenderEngineImp::getInstancePtr()->getRenderEngine()->getBufferManager()->createVertexBuffer(3 * _material->getStride(), Euclid::eUsage_WriteOnly, Euclid::ePool_Manager);
		void* data = _vb->lock(0, 0, Euclid::eLock_Null);
		memcpy(data, vertices, 3 * _material->getStride());
		_vb->unLock();
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 18, Euclid::eFontProperty_Normal, "freeNormal");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_BG_1, "freeBG1");
		_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
		_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
		_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));
	}
	void _render()
	{
		//_renderModel();
		//_renderFont();
	}
	void _renderModel()
	{
		Euclid::Effect* fx = _material->getEffect();
		if (NULL == fx)
		{
			return;
		}
		_material->apply();
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, _material->getStride());
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitive(Euclid::ePrimitive_TriangleList, 0, 1);

			fx->endPass();
		}
		fx->end(); 
	}
	void _renderFont()
	{
		// 文字最后画
		if (_font[Euclid::eFontProperty_Normal])
		{
			std::ostringstream ss;
			ss<<"FPS = "<<_fps;
			_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
		}
	}
	void _clear()
	{
		_material = NULL;
		_vb = NULL;
		for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
		{
			_font[i] = NULL;
		}
		_fps = 0.0f;
	}
	void _calcFPS()
	{
		static float last_time = (float)timeGetTime();
		float current_time = (float)timeGetTime();
		static int count_frames = 0;
		++count_frames;
		float delta = current_time - last_time;
		// calculate FPS per Second
		if(delta >= 1000.0f)
		{
			_fps = (float)count_frames * 1000.0f  / delta;
			last_time = current_time;
			count_frames = 0;
		}
	}

private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
};
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
class CTreeDlg : public CDialogImpl<CTreeDlg>
{
private:
	CTreeViewCtrlEx m_tvCtrl;
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CTreeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
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
								t.AddTail(ffd.cFileName, 0);
							}
						}
					}while (FindNextFile(hFind, &ffd) != 0);
				}
				
			}
		}

	
		return TRUE;
	}
};

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("ModelFrameWindowClass"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CMainWindow()
	{

	}
public:
	
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//
		CreateSimpleToolBar();

		//
		CreateSimpleStatusBar(TEXT("Ready"));
		//
		m_wndSplitVertical.Create ( *this, rcDefault );
	
		//
		mTreeDlg.Create(m_wndSplitVertical, rcDefault);
		m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_LEFT, mTreeDlg);
		//
		mViewWindow.Create(m_wndSplitVertical, rcDefault);
		m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_RIGHT, mViewWindow);
		//
		m_hWndClient = m_wndSplitVertical;
		UpdateLayout();
		/*
			Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
			UpdateLayout() resizes the splitter window to its initial size. 
			If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
			The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
		*/
		m_wndSplitVertical.SetSplitterPos(200);

		//
		return 1;
	}
	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加命令处理程序代码
		CAboutDlg ad;
		ad.DoModal();
		return 0;
	}
private:
	CViewWindow mViewWindow;	
	CTreeDlg mTreeDlg;
	CSplitterWindow m_wndSplitVertical;
};
void CViewWindow::onIdle()
{
	_calcFPS();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_render();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);
}
void CMainWindow::onIdle()
{
	mViewWindow.onIdle();
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.CreateEx();
	mw.ShowWindow(nCmdShow | SW_MAXIMIZE);
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
			mw.onIdle();
		}
	}
		
	return 0;
}
