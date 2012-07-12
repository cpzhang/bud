#include "global.h"

RenderEngineImp::RenderEngineImp( HWND h )
{
	_hwnd = h;
	_render = NULL;
	_initialized = false;
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
