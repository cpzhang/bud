//
#include "Modules.h" 

Modules::Modules()
:_renderEngine(NULL)
{

}


Modules::~Modules()
{

}


Euclid::IRenderEngine* Modules::getRenderEngine()
{
	return _renderEngine;
}

bool Modules::create(HWND hwnd)
{
	_renderEngine = Euclid::createRenderEngine(hwnd);

	//
	if (NULL == _renderEngine)
	{
		return false;
	}

	//
	return true;
}

void Modules::destroy()
{
	if (_renderEngine)
	{
		_renderEngine->destroy();
		delete _renderEngine;
		_renderEngine = NULL;
	}
}