#include "global.h"
#include "AABBEntity.h"
RenderEngineImp::RenderEngineImp( HWND h )
{
	_hwnd = h;
	_render = NULL;
	_initialized = false;
	_clearColor = Euclid::Color::Black;
	_canvas = NULL;
	mIntersection.position = Vec3::ZERO;
	mIntersectionClicked.position = Vec3::ZERO;
	mSkeletonMaterial = NULL;
}

bool RenderEngineImp::create()
{
	_initialized = true;
	_render = Euclid::createRenderEngine();
	//
	if (NULL == _render)
	{
		return false;
	}
	_render->getCreationParameters()->hFocusWindow = _hwnd;
	if (!_render->create())
	{
		return false;
	}

	return true;
}

Euclid::IRenderEngine* RenderEngineImp::getRenderEngine()
{
	return _render;
}

void RenderEngineImp::destroy()
{
	if (mSkeletonMaterial)
	{
		mSkeletonMaterial->destroy();
		delete mSkeletonMaterial;
	}
	AABBEntity::destroyMaterial();
	if (_render)
	{
		_render->destroy();
		delete _render;
		_render = NULL;
	}
}

bool RenderEngineImp::isInitialized()
{
	return _initialized;
}

void RenderEngineImp::setClearColor( Euclid::Color c )
{
	_clearColor = c;
}

Euclid::Color RenderEngineImp::getClearColor()
{
	return _clearColor;
}

void RenderEngineImp::setCanvas( Canvas* c )
{
	_canvas = c;
}

Canvas* RenderEngineImp::getCanvas()
{
	return _canvas;
}

void RenderEngineImp::renderPoint( const Vec3& p, const Euclid::Color& c )
{
	if (mSkeletonMaterial == NULL)
	{
		mSkeletonMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mSkeletonMaterial->setEffect("shader/aP.fx");
	}
	Euclid::Effect* fx = mSkeletonMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mSkeletonMaterial->apply();

	//	
	{
		u32 passes = 0;
		fx->setFloatArray("gDiffuse", &c.r, 4);
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 1, &p, sizeof(Euclid::sPosition));
			fx->endPass();
		}
		fx->end();
	}
}
