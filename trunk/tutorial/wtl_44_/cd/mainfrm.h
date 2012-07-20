// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__AED3894A_1589_4C88_8F02_29BD66C793FE__INCLUDED_)
#define AFX_MAINFRM_H__AED3894A_1589_4C88_8F02_29BD66C793FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

typedef CWinTraits<WS_OVERLAPPED, WS_EX_APPWINDOW> CFloatingWinTraits;

class CMainFrame : 
   public CFrameWindowImpl<CMainFrame, CWindow, CFloatingWinTraits>,
   public CIdleHandler
{
public:
   DECLARE_FRAME_WND_CLASS_EX(NULL, IDR_MAINFRAME, CS_SAVEBITS, COLOR_WINDOW)
   typedef CFrameWindowImpl<CMainFrame, CWindow, CFloatingWinTraits> CFloatingWindowImpl;

   enum { TIMERID = 111 };

   CRgn m_rgn;
   CBitmap m_bmp;
   CSize m_size;
   CFont m_font;

   DWORD m_tStartTime;
   DWORD m_tStopTime;

   virtual BOOL OnIdle()
   {
      return FALSE;
   }

   BEGIN_MSG_MAP(CMainFrame)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_NCHITTEST, OnHitTest)
      MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
      MESSAGE_HANDLER(WM_NCACTIVATE, OnNcActivate)
      MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRightClick)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
      COMMAND_ID_HANDLER(ID_APP_START, OnClockStart)
      COMMAND_ID_HANDLER(ID_APP_STOP, OnClockStop)
      COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
      CHAIN_MSG_MAP(CFloatingWindowImpl)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CMessageLoop* pLoop = _Module.GetMessageLoop();
      pLoop->AddIdleHandler(this);

      CLogFont lf;
      _tcscpy( lf.lfFaceName, _T("Impact"));
      lf.lfHeight = -30;
      lf.SetBold();
      m_font.Attach( lf.CreateFontIndirect() );

      m_size.cx = 200;
      m_size.cy = 80;

      m_tStartTime = GetTickCount()/1000;
      m_tStopTime = m_tStartTime+(60*60);

      m_bmp.LoadBitmap(IDB_BACKGROUND);
      ATLASSERT(!m_bmp.IsNull());

      m_rgn.CreateEllipticRgn(0, 0, m_size.cx, m_size.cy);
      SetWindowRgn(m_rgn,FALSE);

      MoveWindow(0, 0, m_size.cx, m_size.cy, FALSE);
      CenterWindow();

      PostMessage(WM_COMMAND, ID_APP_START, 0);
      return 0;
   }

   LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      PostMessage(WM_CLOSE);
      return 0;
   }

   LRESULT OnHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // NOTE: If the user presses the secondary mouse button, pretend that the
      // user clicked on the client area so that we get WM_CONTEXTMENU messages
      BOOL fPrimaryMouseBtnDown = (::GetAsyncKeyState(::GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) & 0x8000) != 0;
      return fPrimaryMouseBtnDown ? HTCAPTION : HTCLIENT;
   }

   LRESULT OnRightClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      CMenu menuContext;
      menuContext.LoadMenu(IDR_POPUP);
      CPoint pt(lParam);
      ClientToScreen(&pt);
      CMenuHandle menuPopup(menuContext.GetSubMenu(0));
      ::SetMenuDefaultItem(menuPopup, ID_APP_START, FALSE);
      ::SetForegroundWindow(m_hWnd);
      menuPopup.TrackPopupMenu( 
         TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
         pt.x, pt.y, 
         m_hWnd, 
         NULL);
      return 0;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1; // I do the painting around here...
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      ATLTRACE("CMainFrame::OnPaint()\n");
      CPaintDC pdc(m_hWnd); // BeginPaint()...
      CWindowDC dc(m_hWnd);
      CDC dcMem;
      dcMem.CreateCompatibleDC(dc);
      HBITMAP hBmpOld = dcMem.SelectBitmap(m_bmp);
      dc.BitBlt(0, 0, m_size.cx, m_size.cy, dcMem, 0, 0, SRCCOPY);
      dcMem.SelectBitmap(hBmpOld);

      dc.SelectFont(m_font);
      DWORD t = ::GetTickCount()/1000;
      DWORD tDiff = t-m_tStartTime;
      DWORD iHours = tDiff / 60;
      DWORD iMinutes = (tDiff) % 60;
      TCHAR szTime[64];
      ::wsprintf(szTime, _T("%02d:%02d"), iHours, iMinutes);
      dc.SetTextColor(RGB(250,200,100));
      dc.SetBkMode(TRANSPARENT);
      dc.TextOut((m_size.cx/2)-36,22, szTime);

      return 0;
   }

   LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      ATLTRACE("CMainFrame::OnNcActivate()\n");
      // A well-described hack (one of the greatest I've ever seen) by the
      // Windows guru Paul DiLascia.
      // Must call DefWindowProc() - but it repaints the original caption - 
      // so we turn off the visible flag for a brief period.
      DWORD dwStyle = GetStyle();
      if( dwStyle & WS_VISIBLE ) SetWindowLong(GWL_STYLE, (dwStyle & ~ WS_VISIBLE));
      DefWindowProc(uMsg, wParam, lParam);
      if (dwStyle & WS_VISIBLE) SetWindowLong(GWL_STYLE, dwStyle);
      Invalidate();
      return 0;
   }

   LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      ATLTRACE("CMainFrame::OnNcPaint()\n");
      if( IsIconic() ) {
         // If window is minimized we let Windows paint the default taskbar
         // stuff.
         bHandled = FALSE;
         return 0;
      }
      CWindowDC dc(m_hWnd);
      CDC dcMem;
      dcMem.CreateCompatibleDC(dc);
      HBITMAP hBmpOld = dcMem.SelectBitmap(m_bmp);
      dc.BitBlt(0, 0, m_size.cx, m_size.cy, dcMem, 0, 0, SRCCOPY);
      dcMem.SelectBitmap(hBmpOld);
      return 0;
   }

   LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      Invalidate();
      return 0;
   }

   LRESULT OnClockStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      SetTimer(TIMERID, 800);
      return 0;
   }

   LRESULT OnClockStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      KillTimer(TIMERID);
      return 0;
   }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__AED3894A_1589_4C88_8F02_29BD66C793FE__INCLUDED_)
