#include "canvas.h"
#include <sstream>
#include "Entity.h"
#include "Geometry.h"
#include "Group.h"
#include "MzModel.h"
#include "global.h"
#include <cctype>
#include "SubEntity.h"
#include "IObserver.h"
void Canvas::_renderGeometry()
{
	if(_isModelOK)
	{
		if (_model)
		{
			_model->render();
		}
	}

	// 文字最后画
	if (_font[Euclid::eFontProperty_Normal])
	{
		std::ostringstream ss;
		ss<<"正心诚意【修身齐家】格物致知"<<std::endl;
		ss<<"FPS = "<<_fps;
		_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}
}

void Canvas::_clear()
{
	_isOK = false;
	for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
	{
		_font[i] = NULL;
	}
	_fps = 0.0f;
	_clearModel();
}

bool Canvas::_create()
{
	new Buddha::Logger;
	new RenderEngineImp(_hwnd);
	if (!RenderEngineImp::isNull() && !RenderEngineImp::getInstancePtr()->isInitialized())
	{
		RenderEngineImp::getInstancePtr()->create();
	}

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 28, Euclid::eFontProperty_Normal, "freeNormal");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simli.ttf"), 28, Euclid::eFontProperty_BG_1, "freeBG1");
	_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
	_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
	_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));

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
	if (_hwnd == NULL)
	{
		return;
	}
	//
	RECT rect;
	GetClientRect(_hwnd, &rect);
	if (rect.left == rect.right)
	{
		return;
	}
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
	if (!_isInitialized())
	{
		return;
	}
	_destroymodel();
	RenderEngineImp::getInstancePtr()->destroy();
	delete RenderEngineImp::getInstancePtr();

	delete Buddha::Logger::getInstancePtr();
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

void Canvas::update(u32 delta)
{
	if (_isInitialized())
	{
		_camera.update();
	}
	
	if (_model)
	{
		Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix() * _cameraController.getMatrix();
		_model->setMatrix("gWorldViewProj", m);
		_model->setMatrix("gView", _camera.getViewMatrix() * _cameraController.getMatrix());
		_model->setMatrix("gProjection", _camera.getProjectionMatrix());
		//
		_model->update(delta * 0.2);
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
	bHandled = false;
	return 0;
}
LRESULT Canvas::OnLeftButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onLeftButtonDown(x,y);
	bHandled = false;

	return 0;
}
LRESULT Canvas::OnLeftButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onLeftButtonUp(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnRightButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonDown(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnRightButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonUp(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMiddleButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonDown(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMiddleButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonUp(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMouseMove(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	short delta = HIWORD(wParam);
	_cameraController.onMouseWheel(delta);
	bHandled = false;
	return 0;
}

void Canvas::changeModelFile( const std::string& fileName )
{
	size_t dotPos = fileName.find('.');
	if (dotPos == std::string::npos)
	{
		return;
	}
	std::string suffix = fileName.substr(dotPos, fileName.size() - dotPos);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::tolower);
	if (suffix == ".group")
	{
		_destroymodel();
		_clearModel();
		_model = new Group;
	}
	else if (suffix == ".entity")
	{
		_destroymodel();
		_clearModel();
		_model = new Entity;
	} 
	else if(suffix == ".mesh")
	{
		_destroymodel();
		_clearModel();
		_model = new Geometry;
	}
	else if(suffix == ".mz")
	{
		_destroymodel();
		_clearModel();
		_model = new MzModel;
	}
	else if(suffix == ".subentity")
	{
		_destroymodel();
		_clearModel();
		_model = new SubEntity;
	}
	else
	{
		return;
	}

	_model->decode(fileName); 
	_createModel();
	for (size_t i = 0; i != _observers.size(); ++i)
	{
		_observers[i]->update(_model);
	}
}

void Canvas::_clearModel()
{
	_modelFileName.clear();
	_modelMaterialName.clear();
	_modelMeshName.clear();
	_isModelOK = false;
	_textureFiles.clear();
	_model = NULL;
}

bool Canvas::_createModel()
{
	_isModelOK = true;
	return true;
}

void Canvas::_destroymodel()
{
	if (_model)
	{
		_model->destroy();
		delete _model;
		_model = NULL;
	}
}

void Canvas::saveFile( const std::string& fileName )
{
	_model->saveFile(fileName);
}

void Canvas::addObserver( IObserver* o )
{
	_observers.push_back(o);
}

void Canvas::setAnimation( const std::string name )
{
	_model->setAnimationName(name);
}
