#include "game.h"
//
#include "Modules.h"
//
#include "InputMessageHandler.h"
#include "render/Color.h"

//
WaitingForYou::WaitingForYou()
	:_modules(NULL), _material(NULL), _vb(NULL), _fx(NULL), _camera(NULL), _cameraController(NULL), _modelMatrix(Mat4::IDENTITY), _texture(NULL), _vbInstances(NULL), _ib(NULL), _font(NULL)
	,_vertexNumber(0), _mapWidth(0), _fx_axis(NULL), _vertexNumber2GPU(0), _triangleNumber2GPU(0), _lod(0), _water_material(NULL), _rtt(NULL), _texture_skydome(NULL)
{
	_windowTitle = "tutorial";
	_heightMapFileName = "terrain\\height128.raw";
}

WaitingForYou::~WaitingForYou()
{
}

bool WaitingForYou::createModules()
{
	Buddha::StackWalker::GetInstancePtr()->init();
	Buddha::StackWalker::GetInstancePtr()->setDumpCallBack(&_dumpCB);
	_modules = new Modules;
	_dumpCB.onDump();
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
//	render2Texture();
	render2Backbuffer();
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
	//
	//
	_camera = new Euclid::Camera;
	_camera->setPosition(Vec3(0.0f, 200.0f, 0.0f));

	//
	_cameraController = new Euclid::CameraControllerThirdPerson(_camera);

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
	_modelMatrix.makeTransform(Vec3::ZERO, Vec3::UNIT_SCALE * 1.0f, sQ);
	_modelMatrix_skybox.makeTransform(Vec3::ZERO, Vec3::UNIT_SCALE * (_mapWidth / 2.0) * 20.0f, sQ);
	float c = Euler::Basic::Cos(current * 0.0001f);
	_water_matrix.makeTrans(Vec2(c, c));
	//
	return true;
}

bool WaitingForYou::setViewport()
{
	_modules->getRenderEngine()->getRenderSystem()->getViewPort(&_vp);
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
	createVB();
	fillVB();
	//
	createIB();
	fillIB();
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
	// water
	{
		//
		static float scale = 1000;
		//
		_water_vertices[0].position = Vec3(-1.0f, 0.0f, -1.0f) * scale;
		_water_vertices[0].color_ARGB = Euclid::Color::White.getARGB();
		_water_vertices[0].texcoord = Vec2(0.0f, 0.0f);

		_water_vertices[1].position = Vec3(-1.0f, 0.0f, 1.0f) * scale;
		_water_vertices[1].color_ARGB = Euclid::Color::White.getARGB();
		_water_vertices[1].texcoord = Vec2(0.0f, 1.0f);

		_water_vertices[2].position = Vec3(1.0f, 0.0f, 1.0f) * scale;
		_water_vertices[2].color_ARGB = Euclid::Color::White.getARGB();
		_water_vertices[2].texcoord = Vec2(1.0f, 1.0f);

		_water_vertices[3].position = Vec3(1.0f, 0.0f, -1.0f) * scale;
		_water_vertices[3].color_ARGB = Euclid::Color::White.getARGB();
		_water_vertices[3].texcoord = Vec2(1.0f, 0.0f);
	}
	//
	{
		//_water_fx = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionTextureColor.fx");
	}
	{
		_fx_skybox = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionTextureFogLinear.fx");
	}
	{
		_water_material = _modules->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
		_water_material->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionColorTexture);
		Euclid::MaterialVertexTexture* mvt = static_cast<Euclid::MaterialVertexTexture*>(_water_material);
		mvt->setTexture("image/water00.jpg");
	}
	//
	{
		_fx = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionTextureLightmapping.fx");
	}
	//
	
	{
		_fx_axis = _modules->getRenderEngine()->getEffectManager()->createEffectFromFile("shader\\PositionColor.fx");
	}
	//
	{
		_material = _modules->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
		_material->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTexture);
		Euclid::MaterialVertexTexture* mvt = static_cast<Euclid::MaterialVertexTexture*>(_material);
		mvt->setTexture("image/rock.JPG");
		mvt->setLightmapping("image/lightmap.png");
	} 
	//
	{
		_modules->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\simkai.ttf"), 18, Euclid::eFontProperty_Normal, "free");
		_font = _modules->getRenderEngine()->getFontManager()->getFont(std::string("free"));
	}
	//
	createRTT();
	// sky vertices
	{
		int i = 0; int j = 0;
		// +z
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
		// -z
		++i; j = 0;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
		// +x
		++i; j = 0;
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
		// -x
		++i; j = 0;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
		// +y
		++i; j = 0;
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, +0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
		// -y
		++i; j = 0;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(0.f, 0.f); ++j;
		_skybox[i][j].position = Vec3(+0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(0.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, +0.5f); _skybox[i][j].texcoord = Vec2(1.f, 1.f); ++j;
		_skybox[i][j].position = Vec3(-0.5f, -0.5f, -0.5f); _skybox[i][j].texcoord = Vec2(1.f, 0.f);
	}

	{
		_texture_skybox[0] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/+z.jpg");
		_texture_skybox[1] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/-z.jpg");
		//
		_texture_skybox[2] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/-x.jpg");
		_texture_skybox[3] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/+x.jpg");
		//
		_texture_skybox[4] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/+y.jpg");
		_texture_skybox[5] = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/blue/-y.jpg");
	}
	// skydome
	_segment_skydome = 16;
	_ring_skydome = 16;
	{
		float step_segment = Euler::TwoPI / (_segment_skydome - 1);
		float step_ring = 0.5f * Euler::PI / (_ring_skydome - 1);
		float radius = 1000.0f;
		Euclid::sPositionTexture vertex;
		for (u32 ring = 0; ring < _ring_skydome; ++ring)
		{
			float radian_ring = ring * step_ring;
			vertex.position.y = radius * Euler::Basic::Sin(radian_ring);
			vertex.texcoord.y = (float)ring / (float)(_ring_skydome - 1);
			for (u32 segment = 0; segment < _segment_skydome; ++segment)
			{
				float radian_segment = segment * step_segment;
				vertex.position.x = radius * Euler::Basic::Cos(radian_ring) * Euler::Basic::Cos(radian_segment);
				vertex.position.z = radius * Euler::Basic::Cos(radian_ring) * Euler::Basic::Sin(radian_segment);
				vertex.texcoord.x = (float)segment / (float)(_segment_skydome - 1);
				_vertices_skydome.push_back(vertex);
			}
		}
		{
			int offset = 1;
			for (int i = 0; i < _ring_skydome - 1;i += offset)
			{
				for (int j = 0; j < _segment_skydome - 1; j += offset)
				{
					u16 baseIndex = j + i * _segment_skydome;
					{
						//
						_indices_skydome.push_back(baseIndex);
						_indices_skydome.push_back(baseIndex + _segment_skydome * offset);
						_indices_skydome.push_back(baseIndex + offset);
						//
						_indices_skydome.push_back(baseIndex + _segment_skydome * offset);
						_indices_skydome.push_back(baseIndex + offset + _segment_skydome * offset);
						_indices_skydome.push_back(baseIndex + offset);
					}
				}
			}
		}
	}
	{
		_texture_skydome = _modules->getRenderEngine()->getTextureManager()->createTextureFromFile("image/dome/Sky_horiz_1_4096.jpg");
	}
	return true;
}

void WaitingForYou::renderGeometry()
{
	if (_wireframe)
	{
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_WireFrame);
	}
	else
	{
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
	}
	// skydome
	{
#define Show_Skydome
#ifdef Show_Skydome
		// 
		_modules->getRenderEngine()->getRenderSystem()->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTexture);
		//
		{
			u32 passes = 0;
			_fx_skybox->begin(&passes);
			_fx_skybox->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() /** _cameraController->getMatrix()*/); 
			for (int s = 0; s != 6; ++s)
			{
				{
					_fx_skybox->setTexture("g_MeshTexture", _texture_skydome);
				}
				for (u32 i = 0; i != passes; ++i)
				{
					_fx_skybox->beginPass(i);
					//
					_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0, _vertices_skydome.size(), _indices_skydome.size() / 3, &_indices_skydome[0], Euclid::eFormat_Index16, &_vertices_skydome[0], sizeof(Euclid::sPositionTexture));
					_fx_skybox->endPass();
				}
			}
			_fx_skybox->end();
		}
#endif
	}
	// skybox
	{
//#define Show_Skybox
#ifdef Show_Skybox
		// 
		_modules->getRenderEngine()->getRenderSystem()->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTexture);
		//
		{
			static u16 s_water_indices[] = {0, 1, 3, 1, 2, 3};
			u32 passes = 0;
			_fx_skybox->begin(&passes);
			_fx_skybox->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() /** _cameraController->getMatrix()*/ * _modelMatrix_skybox); 
			for (int s = 0; s != 6; ++s)
			{
				{
					_fx_skybox->setTexture("g_MeshTexture", _texture_skybox[s]);
				}
				for (u32 i = 0; i != passes; ++i)
				{
					_fx_skybox->beginPass(i);
					_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0, 4, 2, s_water_indices, Euclid::eFormat_Index16, _skybox[s], sizeof(Euclid::sPositionTexture));
					_fx_skybox->endPass();
				}
			}
			_fx_skybox->end();
		}
#endif
	}
	// water
	{
#define Show_Water
#ifdef Show_Water
		// 
		_water_material->apply();
		//
		{
			static u16 s_water_indices[] = {0, 1, 3, 1, 2, 3};
			u32 passes = 0;
			_water_fx->begin(&passes);
			_water_fx->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() /** _cameraController->getMatrix()*/ * _modelMatrix); 
			if (0 && _isRTTOK)
			{
				_water_fx->setTexture("g_MeshTexture", _rtt);
			}
			else
			{
				_water_fx->setTexture("g_MeshTexture", static_cast<Euclid::MaterialVertexTexture*>(_water_material)->_texture);
			}
			// flow
			{
				_water_fx->setMatrix("g_mTextureMatrix", _water_matrix); 
			}
			for (u32 i = 0; i != passes; ++i)
			{
				_water_fx->beginPass(i);
				_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0, 4, 2, s_water_indices, Euclid::eFormat_Index16, _water_vertices, sizeof(Euclid::sPositionColorTexture));
				_water_fx->endPass();
			}
			_water_fx->end();
		}
#endif
	}
	// terrain
	{
#define Show_Terrain
#ifdef Show_Terrain
		_material->apply();
		_modules->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, sizeof(Euclid::sPositionTexture));
		_modules->getRenderEngine()->getRenderSystem()->setIndices(_ib);
		//
		u32 fm;
		_modules->getRenderEngine()->getRenderSystem()->getRenderState(Euclid::eRenderState_FillMode, &fm);
		if (_wireframe)
		{
			_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_WireFrame);
		}
		else
		{
			_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
		}
		//
		{
			u32 passes = 0;
			_fx->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() /** _cameraController->getMatrix()*/ * _modelMatrix);
			_fx->setMatrix("g_mWorld", _modelMatrix); 
			Vec3 cp = /*_cameraController->getMatrix() * */_camera->getPosition();
			_fx->setFloatArray("g_cameraPositon", &cp.x, 3);
			if (0 && _isRTTOK)
			{
				_fx->setTexture("g_MeshTexture", _rtt);
			}
			else
			{
				_fx->setTexture("g_MeshTexture", static_cast<Euclid::MaterialVertexTexture*>(_material)->_texture);
			}
			_fx->setTexture("g_LightmappingTexture", static_cast<Euclid::MaterialVertexTexture*>(_material)->_lightmapping);
			_fx->begin(&passes);
			for (u32 i = 0; i != passes; ++i)
			{
				_fx->beginPass(i);
				_modules->getRenderEngine()->getRenderSystem()->drawIndexedPrimitive(Euclid::ePrimitive_TriangleList, 0, 0, _vertexNumber2GPU, 0, _triangleNumber2GPU);
				_fx->endPass();
			}
			_fx->end();
		}
		//
		_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, fm);
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
			_fx_axis->setMatrix("g_mWorldViewProjection", _camera->getProjectionMatrix() * _camera->getViewMatrix() /** _cameraController->getMatrix()*/ * _modelMatrix); 
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

	//
	{
#define Show_FPS
#ifdef Show_FPS
		std::ostringstream ss;
		ss<<"FPS:       "<<_fps<<std::endl;
		ss<<"Vertices:  "<<_vertexNumber2GPU<<std::endl;
		ss<<"Triangles: "<<_triangleNumber2GPU<<std::endl;
		ss<<"'L' Lightmapping"<<std::endl;
		ss<<"'T' Wireframe"<<std::endl;
		ss<<"'O' LOD"<<std::endl;
		ss<<"Camera ("<<_camera->getPosition().x<<","<<_camera->getPosition().y<<","<<_camera->getPosition().z<<")"<<std::endl;
		ss<<"Acceleration Levl:"<<Buddha::WindowHelper::getAccelerationLevel()<<std::endl;
		if (_font)
		{
			_modules->getRenderEngine()->getRenderSystem()->setRenderState(Euclid::eRenderState_FillMode, Euclid::eFillMode_Solid);
			_font->render(Vec3(100, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
		}
#endif
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
	if (_water_material)
	{
		_water_material->destroy();
		delete _water_material;
		_water_material = 0;
	}

	if (_texture)
	{
		_texture->release();
		_texture = NULL;
	}
	if (_texture_skydome)
	{
		_texture_skydome->release();
		_texture_skydome = NULL;
	}
	for (int i = 0; i != 6; ++i)
	{
		if (_texture_skybox[i])
		{
			_texture_skybox[i]->release();
			_texture_skybox[i] = NULL;
		}
	}
	//
	if (_rtt)
	{
		_rtt->release();
		_rtt = NULL;
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
		delete _fx;
		_fx = 0;
	}
	//
	if (_fx_skybox)
	{
		_fx_skybox->destroy();
		delete _fx_skybox;
		_fx_skybox = 0;
	}
	//
	if (_fx_axis)
	{
		_fx_axis->destroy();
		delete _fx_axis;
		_fx_axis = 0;
	}
	//
	if (_water_fx)
	{
		_water_fx->destroy();
		delete _water_fx;
		_water_fx = 0;
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
	if (_font)
	{
		// 		_font->destroy();
		// 		delete _font;
		_modules->getRenderEngine()->getFontManager()->destroy();
		_font = NULL;
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
	_wireframe = !_wireframe;
}
// a + a*a + a*a*a + ...
float getSeriesSum(float a, int b)
{
	if(b <= 1)
		return a;
	return Euler::Basic::power(a, b) + getSeriesSum(a, b - 1);
}
void WaitingForYou::toggleLOD()
{
	//
	if (_ib)
	{
		_ib->destroy();
	}
	{
		static int lod = 0;
		int w = _mapWidth - 1;
		if (lod >= 1)
		{
			w -= (int)(_mapWidth * getSeriesSum(0.5, lod));
		}
		int offset = Euler::Basic::power(2, lod);
		_triangleNumber2GPU = w * w * 2;
		if (_ib)
		{
			_ib->create(_triangleNumber2GPU * 3 * sizeof(u16), Euclid::eUsage_WriteOnly, Euclid::eFormat_Index16,Euclid::ePool_Default);
		}
		else
		{
			_ib = _modules->getRenderEngine()->getBufferManager()->createIndexBuffer(_triangleNumber2GPU * 3 * sizeof(u16), Euclid::eUsage_WriteOnly, Euclid::eFormat_Index16,Euclid::ePool_Default);
		}
		
		void* data = _ib->lock(0, 0, Euclid::eLock_Null);
		u16* dataIndex = (u16*)data;
		for (int i = 0; i < _mapWidth - 1;i += offset)
		{
			for (int j = 0; j < _mapWidth - 1; j += offset)
			{
				u16 baseIndex = j + i * _mapWidth;
				u16 indices[] = 
				{
					baseIndex, baseIndex + _mapWidth * offset, baseIndex + offset, 
					baseIndex + _mapWidth * offset, baseIndex + offset + _mapWidth * offset, baseIndex + offset
				};
				memcpy(dataIndex, indices, sizeof(u16) * 6);
				dataIndex += 6;
				
			}
		}
		_ib->unLock();
		++lod;
		if (lod >= 8)
		{
			lod = 0;
		}
	}
}

void WaitingForYou::invalidateDevice()
{
	_modules->getRenderEngine()->getBufferManager()->onInvalidateDevice();
	_modules->getRenderEngine()->getEffectManager()->onInvalidateDevice();
	_modules->getRenderEngine()->getFontManager()->onInvalidateDevice();
	//
	if (_rtt)
	{
		_rtt->release();
		_rtt = NULL;
	}
}

void WaitingForYou::restoreDevice()
{
	_modules->getRenderEngine()->getBufferManager()->onRestoreDevice();
	_modules->getRenderEngine()->getEffectManager()->onRestoreDevice();
	_modules->getRenderEngine()->getFontManager()->onRestoreDevice();
	createRTT();
}

void WaitingForYou::createVB()
{
	//
	std::string heightFilePath = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + _heightMapFileName;
	std::ifstream f(heightFilePath, std::ios::binary);
	if (f.good())
	{
		int length = 0;
		f.seekg(0, std::ios::end);
		length = f.tellg();
		_vertexNumber = length;
		f.seekg(0, std::ios::beg);
		//
		{
			_vb = _modules->getRenderEngine()->getBufferManager()->createVertexBuffer(length * sizeof(Euclid::sPositionTexture), Euclid::eUsage_WriteOnly, Euclid::ePool_Manager);
			_vertexNumber2GPU = length;
			
			int width = Euler::Basic::Sqrt(length);
			_mapWidth = width;
		}
	}
	f.close();
}

void WaitingForYou::fillVB()
{
	//
	std::string heightFilePath = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + _heightMapFileName;
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
			_vertexNumber2GPU = length;
			void* data = _vb->lock(0, 0, Euclid::eLock_Null);
			Euclid::sPositionTexture* dataPos = (Euclid::sPositionTexture*)data;
			int width = Euler::Basic::Sqrt(length);
			_mapWidth = width;
			int half_width = width / 2;
			for (int i = 0; i != width; ++i)
			{
				for (int j = 0; j != width; ++j)
				{
					Euclid::sPositionTexture p;
					p.position.x = (i - half_width) * 10;
					p.position.z = (j - half_width) * 10;
					p.position.y = (buffer[j + i * width]) - 128;
					p.texcoord.x = (float)j / (float)(width - 1);
					p.texcoord.y = (float)i / (float)(width - 1);
					memcpy(dataPos, &p, sizeof(Euclid::sPositionTexture));
					++dataPos;
				}
			}
			_vb->unLock();
		}
		delete buffer;
		buffer = NULL;
	}
	f.close();
}

void WaitingForYou::createIB()
{
	int w = _mapWidth - 1;
	_triangleNumber2GPU = w * w * 2;
	_ib = _modules->getRenderEngine()->getBufferManager()->createIndexBuffer(_triangleNumber2GPU * 3 * sizeof(u16), Euclid::eUsage_WriteOnly, Euclid::eFormat_Index16,Euclid::ePool_Manager);
}

void WaitingForYou::fillIB()
{
	int w = _mapWidth - 1;
	if (_lod >= 1)
	{
		w -= (int)(_mapWidth * getSeriesSum(0.5, _lod));
	}
	_triangleNumber2GPU = w * w * 2;
	int offset = Euler::Basic::power(2, _lod);
	if (++_lod >= 8)
	{
		_lod = 0;
	}

	void* data = _ib->lock(0, 0, Euclid::eLock_Null);
	u16* dataIndex = (u16*)data;
	for (int i = 0; i < _mapWidth - 1;i += offset)
	{
		for (int j = 0; j < _mapWidth - 1; j += offset)
		{
			u16 baseIndex = j + i * _mapWidth;
			u16 indices[] = 
			{
				baseIndex, baseIndex + _mapWidth * offset, baseIndex + offset, 
				baseIndex + _mapWidth * offset, baseIndex + offset + _mapWidth * offset, baseIndex + offset
			};
			memcpy(dataIndex, indices, sizeof(u16) * 6);
			dataIndex += 6;

		}
	}
	_ib->unLock();
}

void WaitingForYou::render2Backbuffer()
{
	_isRTTOK = true;
	//
	_modules->getRenderEngine()->getRenderSystem()->setRenderTarget(0, 0);
	renderImp();
}

void WaitingForYou::render2Texture()
{
	_isRTTOK = false;
	//
	_modules->getRenderEngine()->getRenderSystem()->setRenderTarget(0, _rtt);
	renderImp();
}

void WaitingForYou::renderImp()
{
	//
	_modules->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);

	_modules->getRenderEngine()->getRenderSystem()->beginScene();

	//
	renderGeometry();

	//
	_modules->getRenderEngine()->getRenderSystem()->endScene();
}

void WaitingForYou::createRTT()
{
	_rtt = _modules->getRenderEngine()->getTextureManager()->createEmptyTexture(/*_vp.Width*/256, /*_vp.Height*/256, 1, Euclid::eUsage_RenderTarget, Euclid::eFormat_X8R8G8B8, Euclid::ePool_Default);
}

void WaitingForYou::onKeyDown( char c )
{
	switch(c)
	{
	case 'S':
	case 's':
	case VK_DOWN:
		_camera->moveRelative(Vec3(0,0,2));
		break;
	case 'W':
	case 'w':
	case VK_UP:
		_camera->moveRelative(Vec3(0,0,-2));
		break;
	case 'A':
	case 'a':
	case VK_LEFT:
		_camera->moveRelative(Vec3(-2,0,0));
		break;
	case 'D':
	case 'd':
	case VK_RIGHT:
		_camera->moveRelative(Vec3(2,0,0));
		break;
		//
	case 'R':
	case 'r':
		_camera->moveRelative(Vec3(0,2,0));
		break;
	case 'F':
	case 'f':
		_camera->moveRelative(Vec3(0,-2,0));
		break;
	}
	Vec3 p = _camera->getPosition();
	if (p.y <= 100)
	{
		p.y = 100;
		_camera->setPosition(p);
		_camera->setDirty();
	}
}

float WaitingForYou::getTerrainHeight( float x, float z )
{
	float h;
	return h;
}

const u32 WaitingForYou::sc_PatchSize(17);