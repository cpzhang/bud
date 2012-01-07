#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
#include "render/Color.h"

//
WaitingForYou::WaitingForYou()
	:_modules(NULL), _material(NULL), _vb(NULL), _fx(NULL), _camera(NULL), _cameraController(NULL), _modelMatrix(Mat4::IDENTITY), _texture(NULL), _vbInstances(NULL), _ib(NULL), _font(NULL)
	,_vertexNumber(0), _mapWidth(0), _fx_axis(NULL)
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

	//
	if (!createFonts())
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
	std::string heightFilePath = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "terrain\\height128.raw";
	std::ifstream f(heightFilePath, std::ios::binary);
	if (f.good())
	{
		int length = 0;
		f.seekg(0, std::ios::end);
		length = f.tellg();
		_vertexNumber = length;
		f.seekg(0, std::ios::beg);
		u8* buffer = new u8[length];
		f.read((char*)buffer, length);
		//
		{
			_vb = _modules->getRenderEngine()->getBufferManager()->createVertexBuffer(length * sizeof(Euclid::sPositionTextureNormal), Euclid::eUsage_WriteOnly, Euclid::ePool_Default);
			void* data = _vb->lock(0, 0, Euclid::eLock_Null);
			Euclid::sPositionTextureNormal* dataPos = (Euclid::sPositionTextureNormal*)data;
			int width = Euler::Basic::Sqrt(length);
			_mapWidth = width;
			int half_width = width / 2;
			for (int i = 0; i != width; ++i)
				for (int j = 0; j != width; ++j)
				{
					Euclid::sPositionTextureNormal p;
					p.position.x = i - half_width;
					p.position.z = j - half_width;
					p.position.y = (buffer[j + i * width] - 128) * 0.25f;
					p.texcoord.x = (float)j / (float)(width - 1);
					p.texcoord.y = (float)i / (float)(width - 1);
					p.normal = Vec3(0.0, 1.0, 0.0);
					memcpy(dataPos, &p, sizeof(Euclid::sPositionTextureNormal));
					++dataPos;
				}
				_vb->unLock();
				//
				{
					_ib = _modules->getRenderEngine()->getBufferManager()->createIndexBuffer((width - 1) * (width - 1) * 2 * 3 * sizeof(u16), Euclid::eUsage_WriteOnly, Euclid::eFormat_Index16,Euclid::ePool_Default);
					void* data = _ib->lock(0, 0, Euclid::eLock_Null);
					u16* dataIndex = (u16*)data;
					for (int i = 0; i != width - 1; ++i)
						for (int j = 0; j != width - 1; ++j)
						{
							u16 baseIndex = j + i * width;
							u16 indices[] = {
								baseIndex, baseIndex + width, baseIndex + 1, 
								baseIndex + width, baseIndex + 1 + width, baseIndex + 1};
								memcpy(dataIndex, indices, sizeof(u16) * 6);
								dataIndex += 6;
						}
						_ib->unLock();
				}
		}
		delete buffer;
		buffer = NULL;
	}
	f.close();
	//
	//
	_material = _modules->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
	if (_material)
	{
		_material->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureNormal);
		Euclid::MaterialVertexTexture* mvt = static_cast<Euclid::MaterialVertexTexture*>(_material);
		mvt->setTexture("image/detailMap.tga");
	}

	//
	_fx = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionTextureLight.fx");
	if (NULL == _fx)
	{
		return false;
	}
	//
	_fx_axis = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionColor.fx");
	if (NULL == _fx_axis)
	{
		return false;
	}
	// axis
	{
		//
		static float scale = 100;
		//
		_axis_vertices[0].position = Vec3::ZERO * scale;
		_axis_vertices[0].color_ARGB = Euclid::Color::Red.getARGB();

		_axis_vertices[1].position = Vec3::UNIT_X * scale;
		_axis_vertices[1].color_ARGB = Euclid::Color::Red.getARGB();

		//
		_axis_vertices[2].position = Vec3::ZERO * scale;
		_axis_vertices[2].color_ARGB = Euclid::Color::Green.getARGB();

		_axis_vertices[3].position = Vec3::UNIT_Y * scale;
		_axis_vertices[3].color_ARGB = Euclid::Color::Green.getARGB();

		//
		_axis_vertices[4].position = Vec3::ZERO * scale;
		_axis_vertices[4].color_ARGB = Euclid::Color::Blue.getARGB();

		_axis_vertices[5].position = Vec3::UNIT_Z * scale;
		_axis_vertices[5].color_ARGB = Euclid::Color::Blue.getARGB();
	}
	//
	_camera = new Euclid::Camera;
	_camera->setPosition(Vec3(0.0f, 0.0f, 50.0f));

	//
	_cameraController = new Euclid::CameraControllerThirdPerson(_camera);

	return true;
}

void WaitingForYou::renderGeometry()
{
	//#define Show_FPS
#ifdef Show_FPS
	std::ostringstream ss;
	ss<<_fps;
	if (_font)
	{
		_font->render(Vec3(100, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}
#endif

	// terrain
	{
#define Show_Terrain
#ifdef Show_Terrain
		_material->apply();
		_modules->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, sizeof(Euclid::sPositionTextureNormal));
		_modules->getRenderEngine()->getRenderSystem()->setIndices(_ib);
		//
		{
			u32 passes = 0;
			_fx->setMatrix("g_mWorld", _modelMatrix);
			_fx->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() * _cameraController->getMatrix() * _modelMatrix);
			_fx->setTexture("g_MeshTexture", static_cast<Euclid::MaterialVertexTexture*>(_material)->_texture);
			_fx->begin(&passes);
			for (u32 i = 0; i != passes; ++i)
			{
				_fx->beginPass(i);
				_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitive(Euclid::ePrimitive_TriangleList, 0, 0, _mapWidth * _mapWidth, 0, (_mapWidth - 1) * (_mapWidth - 1) * 2);
				_fx->endPass();
			}
			_fx->end();
		}
#endif
	}
	// axis
	{
#define Show_Axis
#ifdef Show_Axis
		// 
		_modules->getRenderEngine()->getRenderSystem()->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionColor);
		//
		{
			u32 passes = 0;
			_fx_axis->begin(&passes);
			_fx_axis->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() * _cameraController->getMatrix() * _modelMatrix); 
			for (u32 i = 0; i != passes; ++i)
			{
				_fx_axis->beginPass(i);
				_modules->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_LineList, 3, _axis_vertices, sizeof(Euclid::sPositionColor));
				_fx_axis->endPass();
			}
			_fx_axis->end();
		}
#endif
	}
}

bool WaitingForYou::createFonts()
{
	//
	if(_modules->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simkai.ttf"), 18, Euclid::eFontProperty_Normal, "free"))
	{
		_font = _modules->getRenderEngine()->getFontManager()->getFont(std::string("free"));
	}

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
	if (_fx_axis)
	{
		_fx_axis->destroy();
		_fx_axis = 0;
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

void WaitingForYou::toggleFillMode()
{
	static bool b = false;
	if (b)
	{
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_WireFrame);
	}
	else
	{
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
	}
	b = !b;
}
