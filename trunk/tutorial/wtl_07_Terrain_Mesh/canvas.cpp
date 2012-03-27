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

	//
	//RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_WireFrame);
	{
		u32 passes = 0;
		_fxTerrain->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			_fxTerrain->beginPass(i);
			_terrain.render();
			_fxTerrain->endPass();
		}
		_fxTerrain->end();
	}
	//RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
}

void Canvas::_clear()
{
	_material = NULL;
	_vb = NULL;
	_fx = NULL;
	_font = NULL;
	_fps = 0.0f;
	_fx = NULL;
	_fxTerrain = NULL;
	_texTerrain = NULL;
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
	_vb = RenderEngineImp::getInstancePtr()->getRenderEngine()->getBufferManager()->createVertexBuffer(3 * _material->getStride(), Euclid::eUsage_Null, Euclid::ePool_Manager);
	void* data = _vb->lock(0, 0, Euclid::eLock_Null);
	memcpy(data, vertices, 3 * _material->getStride());
	_vb->unLock();

	//
	_fx = RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\Position.fx");

	//
	_fxTerrain = RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionTexture.fx");
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simkai.ttf"), 18, Euclid::eFontProperty_Normal, "free");
	_font = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("free"));

	//
	_terrain.create("terrain\\00.terrain");

	//
	_texTerrain = RenderEngineImp::getInstancePtr()->getRenderEngine()->getTextureManager()->createTextureFromFile("terrain\\00.dds");
	_fxTerrain->setTexture("g_MeshTexture", _texTerrain);
	//
	_camera.setPosition(Vec3(0.0f, 0.0f, 50.0f));
	_cameraController.attachCamera(&_camera);

	//
	RenderEngineImp::getInstancePtr()->setCanvas(this);
	//
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
	if (_fxTerrain)
	{
		_fxTerrain->destroy();
		_fxTerrain = NULL;
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
	if (_texTerrain)
	{
		_texTerrain->destroy();
		_texTerrain = NULL;
	}

	_terrain.destroy();
	RenderEngineImp::getInstancePtr()->destroy();
	delete RenderEngineImp::getInstancePtr();

	//
	_clear();
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
	if (_isInitialized())
	{
		_camera.update();
	}
	
	if (_fxTerrain)
	{
		Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _cameraController.getMatrix();
		_fxTerrain->setMatrix("g_mWorldViewProjection", m);
	}
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

LRESULT Canvas::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	_cameraController.onKeyDown(wParam);
	
	return 0;
}
LRESULT Canvas::OnLeftButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onLeftButtonDown(x,y);

	return 0;
}
LRESULT Canvas::OnLeftButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onLeftButtonUp(x,y);
	return 0;
}

LRESULT Canvas::OnRightButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonDown(x,y);
	return 0;
}

LRESULT Canvas::OnRightButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonUp(x,y);
	return 0;
}

LRESULT Canvas::OnMiddleButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonDown(x,y);
	return 0;
}

LRESULT Canvas::OnMiddleButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonUp(x,y);
	return 0;
}

LRESULT Canvas::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMouseMove(x,y);
	return 0;
}

LRESULT Canvas::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	short delta = HIWORD(wParam);
	_cameraController.onMouseWheel(delta);
	return 0;
}
