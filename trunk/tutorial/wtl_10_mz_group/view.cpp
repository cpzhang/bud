#include "view.h"
#include "global.h"
#include "canvas.h"
LRESULT CView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = false;
	return 0;
}


LRESULT CView::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	
	return 0;
}

LRESULT CView::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;

	return 0;
}

LRESULT CView::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
		RenderEngineImp::getInstancePtr()->getCanvas()->OnKeyDown(uMsg, wParam, lParam, bHandled);
	return 0;
}
LRESULT CView::OnLeftButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnLeftButtonDown(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnLeftButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnLeftButtonUp(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnRightButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnRightButtonDown(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnRightButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnRightButtonUp(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnMiddleButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnMiddleButtonDown(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnMiddleButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnMiddleButtonUp(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnMouseMove(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT CView::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	bHandled = false;
	if(!RenderEngineImp::isNull())
	RenderEngineImp::getInstancePtr()->getCanvas()->OnMouseWheel(uMsg, wParam, lParam, bHandled);
	return 0;
}