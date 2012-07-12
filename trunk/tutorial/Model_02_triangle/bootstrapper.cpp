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

	}
	void _render()
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
	void _clear()
	{
		_material = NULL;
		_vb = NULL;
	}
private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
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
