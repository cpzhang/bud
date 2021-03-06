#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
#include "render/Color.h"

//
WaitingForYou::WaitingForYou()
:_modules(NULL), _material(NULL), _vb(NULL), _fx(NULL), _camera(NULL), _cameraController(NULL), _modelMatrix(Mat4::IDENTITY), _texture(NULL), _vbInstances(NULL), _ib(NULL)
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
	DragAcceptFiles(_hwnd, true);

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
	//sT = current * 0.001f;
	sQ.FromAngleAxis(sT, Vec3::UNIT_Y);
	
	//
	_modelMatrix.makeTransform(Vec3::ZERO, Vec3::UNIT_SCALE, sQ);
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
		_material->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureNormalIndex);
		Euclid::MaterialVertexTexture* mvt = static_cast<Euclid::MaterialVertexTexture*>(_material);
		mvt->setTexture("image/wing.dds");
	}
	// vertices
	{
		Euclid::sPositionTextureNormalIndex vertices[8];
		s32 i = 0;
		//A
		vertices[i].position = Vec3(-10.0f,  10.0f, 0.0f);
		vertices[i].texcoord = Vec2(0.0f, 0.0f);
		vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
		vertices[i].index = 0;
		++i;
		//B
		vertices[i].position = Vec3(-10.0f, -10.0f, 0.0f);
		vertices[i].texcoord= Vec2(0.0f, 1.0f);
		vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
		vertices[i].index = 0;
		++i;
		//C
		vertices[i].position = Vec3(10.0f, 10.0f, 0.0f);
		vertices[i].texcoord = Vec2(1.0f, 0.0f);
		vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
		vertices[i].index = 0;
		++i;
		//D
		vertices[i].position = Vec3(10.0f, -10.0f, 0.0f);
		vertices[i].texcoord= Vec2(1.0f, 1.0f);
		vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
		vertices[i].index = 0;
		++i;

		//

		{		//A
			vertices[i].position = Vec3(-10.0f,  10.0f, 0.0f);
			vertices[i].texcoord = Vec2(0.0f, 0.0f);
			vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
			vertices[i].index = 1;
			++i;
			//B
			vertices[i].position = Vec3(-10.0f, -10.0f, 0.0f);
			vertices[i].texcoord= Vec2(0.0f, 1.0f);
			vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
			vertices[i].index = 1;
			++i;
			//C
			vertices[i].position = Vec3(10.0f, 10.0f, 0.0f);
			vertices[i].texcoord = Vec2(1.0f, 0.0f);
			vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
			vertices[i].index = 1;
			++i;
			//D
			vertices[i].position = Vec3(10.0f, -10.0f, 0.0f);
			vertices[i].texcoord= Vec2(1.0f, 1.0f);
			vertices[i].normal = Vec3(0.0f,  0.0f, 1.0f);
			vertices[i].index = 1;
			++i;
		}
		_vb = _modules->getRenderEngine()->getBufferManager()->createVertexBuffer(8 * sizeof(Euclid::sPositionTextureNormalIndex), Euclid::eUsage_WriteOnly, Euclid::ePool_Default);
		void* data = _vb->lock(0, 0, Euclid::eLock_Null);
		memcpy(data, vertices, 8 * sizeof(Euclid::sPositionTextureNormalIndex));
		_vb->unLock();
	}
	
	// indices
	{
		u16 indices[] = 
		{
			0, 1, 2, 1, 3, 2,
			4, 5, 6, 5, 7, 6
		};
		_ib = _modules->getRenderEngine()->getBufferManager()->createIndexBuffer(12 * sizeof(u16), Euclid::eUsage_WriteOnly, Euclid::eFormat_Index16, Euclid::ePool_Default);
		void* data = _ib->lock(0, 0, Euclid::eLock_Null);
		memcpy(data, indices, 12 * sizeof(u16));
		_ib->unLock();
	}
	// instances vb
	{
		Mat4 matrices[2];
		matrices[1] = Mat4::IDENTITY;
		matrices[0].makeTrans(Vec3(25, 0, 0));
		matrices[0] = matrices[0].transpose();
		//
		_v4s.push_back(Vec4(matrices[0].m[0][0], matrices[0].m[0][1], matrices[0].m[0][2], matrices[0].m[0][3]));
		_v4s.push_back(Vec4(matrices[0].m[1][0], matrices[0].m[1][1], matrices[0].m[1][2], matrices[0].m[1][3]));
		_v4s.push_back(Vec4(matrices[0].m[2][0], matrices[0].m[2][1], matrices[0].m[2][2], matrices[0].m[2][3]));
		_v4s.push_back(Vec4(matrices[0].m[3][0], matrices[0].m[3][1], matrices[0].m[3][2], matrices[0].m[3][3]));
		//
		_v4s.push_back(Vec4(matrices[1].m[0][0], matrices[1].m[0][1], matrices[1].m[0][2], matrices[1].m[0][3]));
		_v4s.push_back(Vec4(matrices[1].m[1][0], matrices[1].m[1][1], matrices[1].m[1][2], matrices[1].m[1][3]));
		_v4s.push_back(Vec4(matrices[1].m[2][0], matrices[1].m[2][1], matrices[1].m[2][2], matrices[1].m[2][3]));
		_v4s.push_back(Vec4(matrices[1].m[3][0], matrices[1].m[3][1], matrices[1].m[3][2], matrices[1].m[3][3]));
		//
	}

	//
	_fx = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader/InstancingShader.fx");
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
	static u32 sInstancesNumber = 2;
	// vertex declaration
	_material->apply();

	_modules->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, sizeof(Euclid::sPositionTextureNormalIndex));
	// render
	{
		_modules->getRenderEngine()->getRenderSystem()->setIndices(_ib);
		//
		u32 passes = 0;
		_fx->setMatrix("g_mViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() * _cameraController->getMatrix());
		static Vec3 sLightPos(0, 10, 10);
		_fx->setFloatArray("g_vLightPosition", &sLightPos[0], 3);
		_fx->setTexture("g_MeshTexture", static_cast<Euclid::MaterialVertexTexture*>(_material)->_texture);

		_fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			_fx->beginPass(i);
			_fx->setVectorArray("g_vInstanceModelMatrix", &_v4s[0], _v4s.size());
			_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitive(Euclid::ePrimitive_TriangleList, 0, 0, 4 * 2, 0, 2 * 2);
			_fx->endPass();
		}
		_fx->end();
	}

	// recover
	//_modules->getRenderEngine()->getRenderSystem()->setStreamSourceFreq(0, 1);
	//_modules->getRenderEngine()->getRenderSystem()->setStreamSourceFreq(1, 1);
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
	if (_vbInstances)
	{
		_vbInstances->destroy();
		delete _vbInstances;
		_vbInstances = NULL;
	}
	//
	if (_ib)
	{
		_ib->destroy();
		delete _ib;
		_ib = NULL;
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
