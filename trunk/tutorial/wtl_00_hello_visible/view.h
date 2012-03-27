// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CView : public CWindowImpl<CView>
{
public:
	DECLARE_WND_CLASS(NULL)

	DWORD m_dwCommCtrlMajor;
	DWORD m_dwCommCtrlMinor;
	bool m_bAlpha;

	CView() :
		m_dwCommCtrlMajor(0),
		m_dwCommCtrlMinor(0),
		m_bAlpha(false)
	{ }

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = false;
		return 0;
	}
};
