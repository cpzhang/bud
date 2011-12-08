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

	//
	_effectFactory = _modules->getRenderEngine()->getEffectFactory();

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
	if (0)
	{

		Euclid::SceneNode* n = _sceneManager->getSceneNode("Quad");
		Euclid::RenderableObject* o = n->getAttachedObject("Quad");
		Euclid::IEffect* e = o->getEffect();
		Mat4& finalMatrix = n->getWorldViewProjectMatrix();

		e->setMatrix("g_mWorldViewProjection", &finalMatrix);

		e->setMatrix("g_mWorld", (Mat4*)&n->_getFullTransform());
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
bool WaitingForYou::_initAxis()
{
	//
	Euclid::ManualObject* m = _sceneManager->createManualObject("Axis");
	if (m)
	{
		//
		m->setVertexDeclarationType(Euclid::VD_POSITION_COLOR);

		m->setRenderQueueOrder(Euclid::eRenderQueueOrder_Line);
		//
		m->setUnitSize(sizeof(Euclid::POSITION_COLOR));

		//
		m->setPrimitiveType(Euclid::ePrimitiveType_LineList);

		// x
		m->addVertex(Vec3(-100.0f, 0.0f, 0.0f), Euclid::Color::Red.getARGB());
		m->addVertex(Vec3( 100.0f, 0.0f, 0.0f), Euclid::Color::Red.getARGB());

		// y
		m->addVertex(Vec3(0.0f, -100.0f, 0.0f), Euclid::Color::Green.getARGB());
		m->addVertex(Vec3(0.0f,  100.0f, 0.0f), Euclid::Color::Green.getARGB());

		// z
		m->addVertex(Vec3(0.0f, 0.0f, -100.0f), Euclid::Color::Blue.getARGB());
		m->addVertex(Vec3(0.0f, 0.0f,  100.0f), Euclid::Color::Blue.getARGB());

		//
		m->create();
		
		//
		m->getRenderSpaceVector()->getComponent("vertex declaration")->setVaule(Euclid::VD_POSITION_COLOR);
		m->setPrimitiveCount(3);
		//
	}

	//
	Euclid::SceneNode* n = _sceneManager->getRootSceneNode()->addSceneNodeChild("Axis");

	//
	n->attachObject(m);

	return true;
}
bool WaitingForYou::initGeometry()
{
	//
	return _initAxis();

	//
	Euclid::ManualObject* m = _sceneManager->createManualObject("Quad");
	if (m)
	{
		//
		m->setVertexDeclarationType(Euclid::VD_POSITION_TEXTURE);

		//
		m->setUnitSize(sizeof(Euclid::POSITION_TEXTURE));

		//
		m->setPrimitiveType(Euclid::ePrimitiveType_TriangleList);

		float length = 50.0f;
		// 
		m->addVertex(Vec3(-length, -length, 0.0f), Vec2(0.0f, 1.0f));
		m->addVertex(Vec3(-length,  length, 0.0f), Vec2(0.0f, 0.0f));
		m->addVertex(Vec3( length,  length, 0.0f), Vec2(1.0f, 0.0f));

		//
		m->addVertex(Vec3( length, -length, 0.0f), Vec2(1.0f, 1.0f));
		m->addVertex(Vec3(-length, -length, 0.0f), Vec2(0.0f, 1.0f));
		m->addVertex(Vec3( length,  length, 0.0f), Vec2(1.0f, 0.0f));

		////////////////////////////////////////
		float z = 50.0f;
		// 
		m->addVertex(Vec3(-length, -length, z), Vec2(0.0f, 1.0f));
		m->addVertex(Vec3(-length,  length, z), Vec2(0.0f, 0.0f));
		m->addVertex(Vec3( length,  length, z), Vec2(1.0f, 0.0f));

		//
		m->addVertex(Vec3( length, -length, z), Vec2(1.0f, 1.0f));
		m->addVertex(Vec3(-length, -length, z), Vec2(0.0f, 1.0f));
		m->addVertex(Vec3( length,  length, z), Vec2(1.0f, 0.0f));
		//
		m->create();

		//
		if (!_renderSystem->queryShaderCaps(3, 0))
		{
			return false;
		}

		Euclid::IEffect* effect = _effectFactory->createEffectFromFile("shader\\Glow.fx");
		effect->setTechnique("Glow");

		Euclid::ITexture* t = _texManager->createTextureFromFile("image\\wing.dds");
		if (t == NULL)
		{
			return false;
		}
		effect->setTexture("g_MeshTexture", t);

		m->setEffect(effect);

		//
		Euclid::SceneNode* n = _sceneManager->getRootSceneNode()->addSceneNodeChild("Quad");

		//
		n->attachObject(m);
	}

	//
	return true;
}

void WaitingForYou::renderGeometry()
{
	_sceneManager->render();

	std::ostringstream buf;
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