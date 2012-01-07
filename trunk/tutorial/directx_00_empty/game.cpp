#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
/*#include "ManualObject.h"*/

//
WaitingForYou::WaitingForYou()
:	_modules(NULL)
{
	_windowTitle = "tutorial";
}

WaitingForYou::~WaitingForYou()
{
}

bool WaitingForYou::createModules()
{
 	_modules = new Modules;
	
	//
	if (!_modules->create(_hwnd))
	{
		return false;
	}

	//
	::ShowCursor(true);

	//
	return true;
}

bool WaitingForYou::foreRender()
{
	//
	return true;
}

bool WaitingForYou::rendering()
{
	//
	//_renderSystem->beginFrame(true, true, false);

	//
	renderGeometry();

	//
	//_renderSystem->endFrame();

	//
	return true;
}

bool WaitingForYou::postRender()
{
	//
	return true;
}

bool WaitingForYou::forePlay()
{
	// call parent
	Buddha::Game::forePlay();

	// input handler
	setInputMessageHandler(new InputMessageHandler);

	// create modules
	if (!createModules())
	{
		return false;
	}

	Record(Buddha::Logger::getInstancePtr()->getPath());

	//
	if (!createFonts())
	{
		return false;
	}

	//
	if (!setViewport())
	{
		return false;
	}

	//
	DragAcceptFiles(_hwnd, true);

	//
	if (!initGeometry())
	{
		return false;
	}

	//
	return true;
}

bool WaitingForYou::update(u32 current, u32 delta)
{
	//
	if (NULL == _modules)
	{
		return false;
	}

	if (!isActive())
	{
		return true;
	}

	//
	return true;
}

bool WaitingForYou::setViewport()
{
	//
// 	D3DVIEWPORT9 v;
// 	v.X = 0;
// 	v.Y = 0;
// 	v.Width = Buddha::WindowHelper::getInstancePtr()->getClientWidth(_hwnd);
// 	v.Height = Buddha::WindowHelper::getInstancePtr()->getClientHeight(_hwnd);
// 	v.MinZ = 0.0f;
// 	v.MaxZ = 1.0f;
// 
// 	//
// 	if (!_modules->getRenderEngine()->getRenderSystem()->setViewport(&v))
// 	{
// 		return false;
// 	}

	//
	return true;
}
bool WaitingForYou::_initAxis()
{
	return true;
}
bool WaitingForYou::initGeometry()
{
	//
	return _initAxis();

	//
	return true;
}

void WaitingForYou::renderGeometry()
{
	
}

bool WaitingForYou::createFonts()
{
	//
	return true;
}

void WaitingForYou::onDragAndDrop()
{
	
}


void WaitingForYou::onF1()
{
}

bool WaitingForYou::destroy()
{
	Game::destroy();

	if (_inputMessageHandler)
	{
		delete _inputMessageHandler;
		_inputMessageHandler = NULL;
	}

	if (_modules)
	{
		_modules->destroy();
		delete _modules;
		_modules = NULL;
	}

	return true;
}