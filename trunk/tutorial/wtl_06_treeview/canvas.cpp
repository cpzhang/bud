#include "canvas.h"
#include "global.h"
#include <sstream>
void Canvas::_renderGeometry()
{
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, _material->getStride());
	_material->apply();
	u32 passes = 0;
	_fx->begin(&passes);
	for (u32 i = 0; i != passes; ++i)
	{
		_fx->beginPass(i);
		//RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitive(Euclid::ePrimitive_TriangleList, 0, 1);
		_fx->endPass();
	}
	_fx->end();

	//
	// vertex declaration
	if (_font)
	{
		std::ostringstream ss;
		ss<<"FPS "<<_fps<<std::endl;
		_font->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}

}

void Canvas::_clear()
{
	_material = NULL;
	_vb = NULL;
	_fx = NULL;
	_font = NULL;
	_fps = 0.0f;
}

bool Canvas::_create()
{
	new RenderEngineImp(_hwnd);
	if (!RenderEngineImp::isNull() && !RenderEngineImp::getInstancePtr()->isInitialized())
	{
		RenderEngineImp::getInstancePtr()->create();
	}
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

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simkai.ttf"), 18, Euclid::eFontProperty_Normal, "free");
	_font = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("free"));

	return true;
}

bool Canvas::_isInitialized()
{
	return _material != NULL && _vb != NULL;
}

void Canvas::render()
{
	if (!_isInitialized())
	{
		_create();
	}
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		return;
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, RenderEngineImp::getInstancePtr()->getClearColor(), 1.0f, 0L);

	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_renderGeometry();

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);

	//
	_calcFPS();
}

void Canvas::destroy()
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
}

void Canvas::onSize( int w, int h )
{
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		return;
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onInvalidateDevice();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->onInvalidateDevice();
	//
	Euclid::sViewPort vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = w;
	vp.Height = h;
	vp.MinZ = 0;
	vp.MaxZ = 1;
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setViewport(&vp);

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->onRestoreDevice();
}

bool Canvas::create()
{
	return true;
}

Canvas::Canvas( HWND h )
{
	_hwnd = h;
	_clear();
}

void Canvas::update()
{

}

void Canvas::_calcFPS()
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
