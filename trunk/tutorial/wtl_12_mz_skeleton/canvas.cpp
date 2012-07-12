#include "canvas.h"
#include <sstream>
void Canvas::_renderGeometry()
{
	if (_vb == NULL || _material == NULL)
	{
		return;
	}
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
	//RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_WireFrame);
	if(_isTerrainOK)
	{
		if (_group)
		{
			_group->render();
		}
	}

	// 文字最后画
	// vertex declaration
	if (_font[Euclid::eFontProperty_Normal])
	{
		std::ostringstream ss;
		ss<<"正心诚意【修身齐家】格物致知"<<std::endl;
		if (0 && _isTerrainOK)
		{
			ss<<"vertices: "<<_terrain->getVertexNumber()<<std::endl;
			ss<<"faces: "<<_terrain->getFaceNumber()<<std::endl;
		}
		//_font[Euclid::eFontProperty_BG_1]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::White, ss.str());
		//_font[Euclid::eFontProperty_BG_1]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::White, ss.str());
		_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}
	//RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
}

void Canvas::_clear()
{
	_isOK = false;
	_material = NULL;
	_vb = NULL;
	_fx = NULL;
	for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
	{
		_font[i] = NULL;
	}
	_fps = 0.0f;
	_fx = NULL;
	_fxTerrain = NULL;
	_clearTerrain();
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
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 16, Euclid::eFontProperty_Normal, "freeNormal");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 28, Euclid::eFontProperty_BG_1, "freeBG1");
	_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
	_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
	_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));

	//
	_fxTerrain = RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\aPT.fx");

	//
	_camera.setPosition(Vec3(0.0f, 0.0f, 50.0f));
	_cameraController.attachCamera(&_camera);

	//
	RenderEngineImp::getInstancePtr()->setCanvas(this);
	_isOK = true;
	//
	return true;
}

bool Canvas::_isInitialized()
{
	return _isOK;
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
	_destroyTerrain();
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
	
	if (_group)
	{
		Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _cameraController.getMatrix();
		_group->setMatrix("gWorldViewProj", m);
		//_group->setMatrix("gWorld", m);
		_group->setMatrix("gView", _camera.getViewMatrix() * _cameraController.getMatrix());
		_group->setMatrix("gProjection", _camera.getProjectionMatrix());
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

void Canvas::changeTerrainFile( const std::string& fileName )
{
	_destroyTerrain();
	_clearTerrain();
	size_t dotPos = fileName.find('.');
	if (dotPos == std::string::npos)
	{
		return;
	}
	std::string suffix = fileName.substr(dotPos, fileName.size() - dotPos);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::tolower);
	if (suffix == ".group")
	{
		_group = new Group;
	}
	else if (suffix == ".entity")
	{
		_group = new Entity;
	} 
	else if(suffix == ".mesh")
	{
		_group = new Geometry;
	}
	else if(suffix == ".mz")
	{
		_group = new MzModel;
	}
	else
	{
		return;
	}
	
	_group->decode(fileName); 
	_createTerrain();
}

void Canvas::_clearTerrain()
{
	_texTerrain = NULL;
	_terrainFileName.clear();
	_terrainMaterialName.clear();
	_terrainMeshName.clear();
	_isTerrainOK = false;
	_terrain = NULL;
	_textureFiles.clear();
	_group = NULL;
}

bool Canvas::_createTerrain()
{
	_isTerrainOK = true;
	return true;
}

void Canvas::_destroyTerrain()
{
	if (_texTerrain)
	{
		_texTerrain->destroy();
		_texTerrain = NULL;
	}
	if (_terrain)
	{
		_terrain->destroy();
		delete _terrain;
		_terrain = NULL;
	}
	if (_group)
	{
		_group->destroy();
		delete _group;
		_group = NULL;
	}
}

void Canvas::saveFile( const std::string& fileName )
{
	_group->saveFile(fileName);
}
