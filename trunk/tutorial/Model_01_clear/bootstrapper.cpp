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
		MESSAGE_HANDLER(WM_SIZE, onSize)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
	END_MSG_MAP()

	IdleHandler_Derived

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
		}

		//
		return 1;
	}
	LRESULT onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) 
	{
		b = TRUE;
		if (RenderEngineImp::isNull())
		{
			return 1;
		}
		if (NULL == RenderEngineImp::getInstancePtr()->getRenderEngine())
		{
			RenderEngineImp::getInstancePtr()->_width = LOWORD(lParam);
			RenderEngineImp::getInstancePtr()->_height = HIWORD(lParam);
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
			RenderEngineImp::getInstancePtr()->destroy();
			delete RenderEngineImp::getInstancePtr();
		}

		//
		return 1;
	}
};
class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	//enum { IDD = IDD_ABOUT };
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
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
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CMainWindow()
	{

	}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//
		CreateSimpleToolBar();
		//
		m_hWndClient = mViewWindow.Create(m_hWnd, rcDefault);
		//
		return 1;
	}
private:
	CViewWindow mViewWindow;	
};
void CViewWindow::onIdle()
{
	if (RenderEngineImp::isNull()) 
	{
		return;
	}
	if (!RenderEngineImp::getInstancePtr()->isInitialized())
	{
		if (RenderEngineImp::getInstancePtr()->_width > 0)
		{
			RenderEngineImp::getInstancePtr()->create();
		}
		else
		{
			return;
		}
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	
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
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// Ensures that the common control DLL (Comctl32.dll) is loaded, and registers specific common control classes from the DLL.
	// An application must call this function before creating a common control.
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);

	CMainWindow mw;
	mw.GetWndClassInfo().m_wc.style |= (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
	mw.CreateEx();
	mw.ShowWindow(nCmdShow | SW_SHOWDEFAULT);
	mw.UpdateWindow();
	MSG m;
	::ZeroMemory(&m, sizeof(m));
//	mw.RedrawWindow();
//	mw.ShowWindow(SW_MAXIMIZE);
//	mw.UpdateWindow();
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
	_Module.Term();
	//Closes the COM library on the current thread, unloads all DLLs loaded by the thread, 
	//frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
	::CoUninitialize();

	return 0;
}
