#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
/*#include "ManualObject.h"*/

//
WaitingForYou::WaitingForYou()
:	_modules(NULL), _renderSystem(NULL), _sceneManager(NULL), _bufferManager(NULL),
	_fontManager(NULL), _freeTypeFont("Default Font"), _model(NULL), _texManager(NULL)
{
	_windowTitle = "tutorial_shader_introduction";
	_blooding = 0;
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
	_renderSystem = _modules->getRenderEngine()->getRenderSystem();

	//
	_sceneManager = _modules->getRenderEngine()->getSceneManager();
	//

	//
	_bufferManager = _modules->getRenderEngine()->getBufferFactory();

	//
	_fontManager = _modules->getRenderEngine()->getFontFactory();

	//
	_zipManager = _modules->getRenderEngine()->getZIPManager();

	//
	_texManager = _modules->getRenderEngine()->getTextureFactory();


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
	_renderSystem->beginFrame(true, true, false);

	//
	renderGeometry();

	//
	//_renderCEGUI();

	//
	_renderSystem->endFrame();

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
	D3DVIEWPORT9 v;
	v.X = 0;
	v.Y = 0;
	v.Width = Buddha::WindowHelper::getInstancePtr()->getClientWidth(_hwnd);
	v.Height = Buddha::WindowHelper::getInstancePtr()->getClientHeight(_hwnd);
	v.MinZ = 0.0f;
	v.MaxZ = 1.0f;

	//
	if (!_modules->getRenderEngine()->getRenderSystem()->setViewport(&v))
	{
		return false;
	}

	//
	return true;
}

bool WaitingForYou::initGeometry()
{
	//
	return true;
}

void WaitingForYou::renderGeometry()
{
	std::ostringstream buf;
	//buf<<"hello world";
	buf<<"FPS : "<<_fps;

	_fontManager->getFont()->render(Euclid::FontDimention_2D, D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());
}

bool WaitingForYou::createFonts()
{
	if (NULL == _fontManager)
	{
		return false;
	}

	//
	std::string faceFile("c:\\WINDOWS\\Fonts\\times.ttf");
	
	//
	bool b = _fontManager->createFont(Euclid::FontType_FreeType, faceFile, 14, Euclid::FONT_PROP_NONE, _freeTypeFont);

	//
	return b;
}

void WaitingForYou::onDragAndDrop()
{
	if (_model)
	{
		_model->destroy();
	}

	_model = _sceneManager->createEntityMD2("model", _dragDropFile);
	_node->reset();
	_renderSystem->resetCamera();
	_node->attachObject(_model);
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