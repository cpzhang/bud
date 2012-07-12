#include <atlbase.h>
#include <atlapp.h>
#include "resource.h"

class CColoredStatic : public CWindowWithReflectorImpl< CColoredStatic, CWindow,  CControlWinTraits> 
{
public:
	DECLARE_WND_SUPERCLASS(0, _T("STATIC"))

	CColoredStatic( COLORREF foreground, COLORREF background ) 
	{
		m_foreground = foreground;
		m_background = background;
		m_backgroundBrush = ::CreateSolidBrush( m_background );
	}

	~CColoredStatic( ) 
	{
		::DeleteObject( m_backgroundBrush );
	}
private:

	BEGIN_MSG_MAP(CColoredStatic)
		MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnControlColorStatic)
	END_MSG_MAP()

	LRESULT OnControlColorStatic(UINT nMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled) 
	{
		HDC hdc = reinterpret_cast< HDC >( wParam );
		::SetTextColor(hdc, m_foreground );
		::SetBkColor( hdc, m_background );
		return reinterpret_cast< LRESULT >(m_backgroundBrush);
	}

	COLORREF m_foreground;
	COLORREF m_background;
	HBRUSH m_backgroundBrush;
};

template <typename Deriving>
class CFileHandler 
{
public:
	// Message map in base class
	BEGIN_MSG_MAP(CFileHandler)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
	END_MSG_MAP()

	LRESULT OnFileNew(WORD, WORD, HWND, BOOL&)
	{
		return TRUE;
	}
};

class CMainWindow : 
	public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>,
	public CFileHandler<CMainWindow>
{
public:
	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_QUIT, onQuit)
		COMMAND_HANDLER(ID_FILE_NEW, 0, onNew)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, onOpen)
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnControlColorStatic)

		//REFLECT_NOTIFICATIONS()
		//FORWARD_NOTIFICATIONS()
		CHAIN_MSG_MAP(CFileHandler<CMainWindow>)
	END_MSG_MAP()

public:
	CMainWindow()
		:mCStatic( RGB( 0, 0, 0), RGB(255, 0, 0) ), mMessage( RGB( 0, 0, 255), RGB(0, 255, 0) )
	{

	}
private:
	BOOL onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		rc.left = 25;
		rc.right = 300;
		rc.top = 25;
		rc.bottom = 75;

		mMessage.Create(m_hWnd, rc, TEXT("Window"));

		rc.left = 250;
		rc.right = 500;
		rc.top = 125;
		rc.bottom = 175;
		mCStatic.Create(m_hWnd, rc, TEXT("Imp"));
		mBrushBackground = ::CreateSolidBrush(RGB(0, 0, 255));
		bHandled = false;
		return TRUE;
	}
	BOOL onDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = false;
		return TRUE;
	}
	BOOL onQuit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = false;
		return TRUE;
	}
	LRESULT onNew(WORD wNotifyCode, WORD wID, HWND hWndCtl,
		BOOL& bHandled)
	{
		bHandled = false;
		return TRUE;
	}
	LRESULT onOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl,
		BOOL& bHandled)
	{
		return TRUE;
	}
	LRESULT OnControlColorStatic(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
	{
		bHandled = false;
		HDC hdc = reinterpret_cast< HDC >( wParam );
		::SetTextColor(hdc, RGB(255, 0, 0) );
		::SetBkColor( hdc, RGB(0, 255, 0 ) );
		return reinterpret_cast< LRESULT >(mBrushBackground);
	}
	void OnFinalMessage(HWND hWnd) 
	{
		::DeleteObject( mBrushBackground );
		PostQuitMessage(0);
	}

private:
	CColoredStatic mMessage;
	HBRUSH mBrushBackground;
	CColoredStatic mCStatic;
};
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{

	CMainWindow mw;
	CMainWindow::GetWndClassInfo().m_wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	mw.Create(NULL);
	mw.CenterWindow();
	mw.ShowWindow(nCmdShow);
	mw.UpdateWindow();
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	for (;;)
	{
		if (::PeekMessage(&m, NULL, 0, 0, PM_REMOVE))
		{
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
