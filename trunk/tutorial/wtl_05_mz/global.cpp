#include "global.h"

RenderEngineImp::RenderEngineImp( HWND h )
{
	_hwnd = h;
	_render = NULL;
	_initialized = false;
	_clearColor = Euclid::Color::Black;
	_canvas = NULL;
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
