// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include "render/Euclid.h"
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
	{
		_clear();
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
private:
	void _renderGeometry();
	void _clear();
	bool _create();
	bool _isInitialized();
	//
private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::Effect*			_fx;
};
