#include "gameImp.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"

//
GameImp::GameImp()
:	_modules(NULL), _font(NULL)
{
	_windowTitle = "tutorial";
}

GameImp::~GameImp()
{
}

bool GameImp::createModules()
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

bool GameImp::foreRender()
{
	//
	return true;
}

bool GameImp::rendering()
{
	//
	_modules->getRenderEngine()->getRenderSystem()->setRenderTarget(0, 0);
	_modules->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);

	_modules->getRenderEngine()->getRenderSystem()->beginScene();

	//
	renderGeometry();

	//
	_modules->getRenderEngine()->getRenderSystem()->endScene();
	_modules->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL); 
	//
	return true;
}

bool GameImp::postRender()
{
	//
	return true;
}

bool GameImp::forePlay()
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
	//DragAcceptFiles(_hwnd, true);

	//
	if (!initGeometry())
	{
		return false;
	}
	// 
	if (!_initNet())
	{
		return false;
	}
	//
	return true;
}

bool GameImp::update(u32 current, u32 delta)
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

bool GameImp::setViewport()
{
	//
	return true;
}
bool GameImp::_initAxis()
{
	return true;
}
bool GameImp::initGeometry()
{
	//
	return _initAxis();

	//
	return true;
}

void GameImp::renderGeometry()
{
	std::ostringstream ss;
	ss<<"FPS:       "<<_fps<<std::endl;
	if (_font)
	{
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
		_font->render(Vec3(100, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}

}

bool GameImp::createFonts()
{
	//
	{
		_modules->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simkai.ttf"), 18, Euclid::eFontProperty_Normal, "free");
		_font = _modules->getRenderEngine()->getFontManager()->getFont(std::string("free"));
	}

	//
	return true;
}

void GameImp::onDragAndDrop()
{
	
}


void GameImp::onF1()
{
}

bool GameImp::destroy()
{
	Game::destroy();
	_destroyNet();
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

bool GameImp::_initNet()
{
	WSAData d;
	int ret = WSAStartup(MAKEWORD(2, 2), &d);
	return ret == 0;
}

void GameImp::_destroyNet()
{
	WSACleanup();
}
