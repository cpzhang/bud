#include "view.h"
#include "global.h"
LRESULT CView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		return 0;
	}
	if (!_isInitialized())
	{
		_create();
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);

	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_renderGeometry();

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);

	return 0;
}

void CView::_renderGeometry()
{
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, _material->getStride());
	_material->apply();
	u32 passes = 0;
	_fx->begin(&passes);
	for (u32 i = 0; i != passes; ++i)
	{
		_fx->beginPass(i);
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitive(Euclid::ePrimitive_TriangleList, 0, 1);
		_fx->endPass();
	}
	_fx->end();
}

void CView::_clear()
{
	_material = NULL;
	_vb = NULL;
	_fx = NULL;
}

bool CView::_create()
{
	//
	_material = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
	
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
	_fx = RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\Position.fx");

	return true;
}

bool CView::_isInitialized()
{
	return _material != NULL && _vb != NULL;
}

LRESULT CView::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	//
	if (_fx)
	{
		_fx->destroy();
		_fx = 0;
	}

	//
	if (_material)
	{
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

	bHandled = false;
	
	return 0;
}

LRESULT CView::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled )
{
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		goto out;
	}
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
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice();
out:
	//
	bHandled = false;

	return 0;
}
