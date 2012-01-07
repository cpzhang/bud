#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
#include "render/Color.h"

//
WaitingForYou::WaitingForYou()
:_modules(NULL), _material(NULL), _vb(NULL), _fx(NULL), _camera(NULL), _cameraController(NULL), _modelMatrix(Mat4::IDENTITY), _texture(NULL)
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
//	DragAcceptFiles(_hwnd, true);

	//
	if (!initGeometry())
	{
		return false;
	}

	setProcess(Buddha::eProcess_CreateModules);
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

	if (_camera)
	{
		_camera->update();
	}

	// rotate
	static Quaternion sQ;
	static float sT;
	sT = current * 0.001f;
	sQ.FromAngleAxis(sT, Vec3::UNIT_Y);
	
	//
	//_modelMatrix.makeTransform(Vec3::ZERO, Vec3::UNIT_SCALE, sQ);
	//
	return true;
}

bool WaitingForYou::setViewport()
{
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
	_material = _modules->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
	if (_material)
	{
		_material->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTexture);
		Euclid::MaterialVertexTexture* mvt = static_cast<Euclid::MaterialVertexTexture*>(_material);
		//mvt->setTexture("image/wing.dds");
		mvt->setTexture("image/setting.png");
	}
	//
	Euclid::sPositionTexture vertices[6];
	//A
	vertices[0].position = Vec3(-10.0f,  10.0f, 0.0f);
	vertices[0].texcoord = Vec2(0.0f, 0.0f);
	//B
	vertices[1].position = Vec3(-10.0f, -10.0f, 0.0f);
	vertices[1].texcoord= Vec2(0.0f, 1.0f);
	//C
	vertices[2].position = Vec3(10.0f, 10.0f, 0.0f);
	vertices[2].texcoord = Vec2(1.0f, 0.0f);

	//B
	vertices[3].position = Vec3(-10.0f, -10.0f, 0.0f);
	vertices[3].texcoord = Vec2(0.0f, 1.0f);
	//D
	vertices[4].position = Vec3(10.0f, -10.0f, 0.0f);
	vertices[4].texcoord= Vec2(1.0f, 1.0f);
	//C
	vertices[5].position = Vec3(10.0f, 10.0f, 0.0f);
	vertices[5].texcoord = Vec2(1.0f, 0.0f);

	_vb = _modules->getRenderEngine()->getBufferManager()->createVertexBuffer(6 * sizeof(Euclid::sPositionTexture), Euclid::eUsage_WriteOnly, Euclid::ePool_Default);
	void* data = _vb->lock(0, 0, Euclid::eLock_Null);
	memcpy(data, vertices, 6 * sizeof(Euclid::sPositionTexture));
	_vb->unLock();

	//
	_fx = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader/PositionTexture.fx");
	if (NULL == _fx)
	{
		return false;
	}

	//
	_camera = new Euclid::Camera;
	_camera->setPosition(Vec3(0.0f, 0.0f, 100.0f));

	//
	_cameraController = new Euclid::CameraControllerThirdPerson(_camera);

	//
	return _initAxis();

	//
	return true;
}

void WaitingForYou::renderGeometry()
{
	_modules->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, sizeof(Euclid::sPositionTexture));
	_material->apply();
	{
		u32 passes = 0;
		_fx->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() * _cameraController->getMatrix() * _modelMatrix);
		_fx->setTexture("g_MeshTexture", static_cast<Euclid::MaterialVertexTexture*>(_material)->_texture);

		_fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			_fx->beginPass(i);
			_modules->getRenderEngine()->getRenderSystem()->drawPrimitive(Euclid::ePrimitive_TriangleList, 0, 2);
			_fx->endPass();
		}
		_fx->end();
	}
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

	if (_material)
	{
		_material->destroy();
		delete _material;
		_material = 0;
	}

	if (_texture)
	{
		_texture->release();
		_texture = NULL;
	}

	//
	if (_camera)
	{
		delete _camera;
		_camera = NULL;
	}

	//
	if (_cameraController)
	{
		delete _cameraController;
		_cameraController = NULL;
	}

	//
	if (_fx)
	{
		_fx->destroy();
		_fx = 0;
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
	//
	if (_modules)
	{
		_modules->destroy();
		delete _modules;
		_modules = NULL;
	}

	return true;
}

Euclid::CameraControllerThirdPerson* WaitingForYou::getCameraController()
{
	return _cameraController;
}
