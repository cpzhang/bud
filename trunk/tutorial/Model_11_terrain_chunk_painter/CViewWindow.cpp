#include "CViewWindow.h"
#include "CAboutDlg.h"
#include "MzModel.h"
CViewWindow::CViewWindow()
	{
		_clear();
	}
	LRESULT CViewWindow::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		static float sf_step = 1.0f;
		switch(wParam)
		{
		case 'W':
			{
				_cameraRelativeShpereCoordination.mOrigion.z -= sf_step;
			}break;
		case 'S':
			{
				_cameraRelativeShpereCoordination.mOrigion.z += sf_step;
			}break;
		case 'A':
			{
				_cameraRelativeShpereCoordination.mOrigion.x -= sf_step;
			}break;
		case 'D':
			{
				_cameraRelativeShpereCoordination.mOrigion.x += sf_step;
			}break;
		}

		bHandled = false;
		return 0;
	}
	LRESULT CViewWindow::onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		new Buddha::Logger;
		new RenderEngineImp(m_hWnd);
		//
		return 1;
	}
	LRESULT CViewWindow::onPaint(UINT, WPARAM, LPARAM, BOOL& b) 
	{
		_painted = true;
		b = FALSE;
		if (!RenderEngineImp::getInstancePtr()->isInitialized())
		{
			RenderEngineImp::getInstancePtr()->create();
			_create();
		}

		//
		return 1;
	}
	LRESULT CViewWindow::onMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL& b) 
	{
		//
		float delta = ( short )HIWORD( wParam );
		delta /= 120.0f;
		_cameraRelativeShpereCoordination.updateDistance(delta * 0.1f);
		//
		return 1;
	}
	LRESULT CViewWindow::onMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& b) 
	{
		//获得焦点
		SetFocus();
		//
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		static Vec2 p(x, y);
		//射线拾取，更新地形圆位置
		{
			Ray r = _camera.getCameraToViewportRay(x, y);

			//与xz平面的交点
			Real t = -r._origin.y / r._direction.y;
			Vec3 p = r.getPoint(t);
			mSculptor.move(p, &gTerrain);
		}
		Vec2 currentPos(x, y);
		bool isControlDown = ::GetAsyncKeyState(VK_CONTROL) & 0x8000f;
		bool isLButtonDown = ::GetAsyncKeyState(VK_LBUTTON) & 0x8000f;
		bool isAltDown = ::GetAsyncKeyState(VK_MENU) & 0x8000f;
		bool isShiftDown = ::GetAsyncKeyState(VK_SHIFT) & 0x8000f;
		if (isLButtonDown && isAltDown)
		{
			Vec2 delta = currentPos - p;
			delta *= 0.01f;
			_cameraRelativeShpereCoordination.mAngleAroundY += delta.x;
			_cameraRelativeShpereCoordination.updateAngleUpFromXZ(delta.y);
			p = currentPos;
		}
		else if (isLButtonDown && isControlDown)
		{
			gTerrain.raise(&mSculptor, true);
		}
		else if (isLButtonDown && isShiftDown)
		{
			gTerrain.raise(&mSculptor, false);
		}
		else
		{
			p = currentPos;
		}
		//
		{
			static Vec2 p(x, y);
			Vec2 currentPos(x, y);
			if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000f)
			{
				Vec2 delta = currentPos - p;
				delta *= 0.01f;
				_light.mPosition.mAngleAroundY += delta.x;
				_light.mPosition.updateAngleUpFromXZ(delta.y);
				p = currentPos;
			}
			else
			{
				p = currentPos;
			}
		}

		//
		return 1;
	}
	LRESULT CViewWindow::onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) 
	{
		b = TRUE;
		if (RenderEngineImp::isNull())
		{
			return 1;
		}
		if (NULL == RenderEngineImp::getInstancePtr()->getRenderEngine())
		{
			RenderEngineImp::getInstancePtr()->_width = LOWORD(lParam);
			RenderEngineImp::getInstancePtr()->_height = HIWORD(lParam);
			return 1;
		}

		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onInvalidateDevice();
		//
		Euclid::sViewPort vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = LOWORD(lParam);
		vp.Height = HIWORD(lParam);
		vp.MinZ = 0;
		vp.MaxZ = 1;
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setViewport(&vp);
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice(); 
		//
		return 1;
	}
	LRESULT CViewWindow::onDestroy(UINT, WPARAM, LPARAM, BOOL&) 
	{
		mSculptor.destroy();
		gTerrain.destroy();
		delete EventManager::getInstancePtr();
		if (RenderEngineImp::getInstancePtr()->isInitialized())
		{
			_sphere.destroy();
			if (_material)
			{
				_material->destroy();
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
			_destroymodel();
			RenderEngineImp::getInstancePtr()->destroy();
			delete RenderEngineImp::getInstancePtr();
		}

		delete Buddha::Logger::getInstancePtr();
		//
		return 1;
	}
	void CViewWindow::_create()
	{
		//
		_material = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		_material->setEffect("shader\\Position.fx");
		//
		Euclid::sPosition vertices[3];
		vertices[0].position = Vec3(-1.0f, 0.0f, 0.0f);
		vertices[1].position = Vec3(0.0f, 1.0f, 0.0f);
		vertices[2].position = Vec3(1.0f, 0.0f, 0.0f);
		_vb = RenderEngineImp::getInstancePtr()->getRenderEngine()->getBufferManager()->createVertexBuffer(3 * _material->getStride(), Euclid::eUsage_WriteOnly, Euclid::ePool_Manager);
		void* data = _vb->lock(0, 0, Euclid::eLock_Null);
		memcpy(data, vertices, 3 * _material->getStride());
		_vb->unLock();
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 16, Euclid::eFontProperty_Normal, "freeNormal");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_BG_1, "freeBG1");
		_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
		_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
		_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));
		//	
		_sphere.create();
		//
		mSculptor.create();
		//
		EventManager::getInstancePtr()->subscribeEvent(eEvent_SculptorRadiusChanged, SubscriberSlot(&CViewWindow::updateSculptorRadius, this));
		EventManager::getInstancePtr()->subscribeEvent(eEvent_SculptorStrengthChanged, SubscriberSlot(&CViewWindow::updateSculptorStrength, this));
	}
	bool CViewWindow::updateSculptorRadius(const EventArgs& e)
	{
		EventArgsSculptorRadiusChanged args = (EventArgsSculptorRadiusChanged&)e;
		mSculptor.generateMesh(args.mRadius, &gTerrain);
		return true;
	}
	bool CViewWindow::updateSculptorStrength(const EventArgs& e)
	{
		EventArgsSculptorStrengthChanged args = (EventArgsSculptorStrengthChanged&)e;
		mSculptor.mParaA = args.mStrength;
		return true;
	}
	void CViewWindow::_render()
	{
		_renderTerrain();
		mSculptor.render();
		_renderModel();
		//_renderSphere();
		_renderLight();
		_renderFont();
	}
	void CViewWindow::_renderTerrain()
	{
		gTerrain.render();
	}
	void CViewWindow::_renderSphere()
	{
		_sphere.render();
	}
	void CViewWindow::_renderLight()
	{
		Euclid::sPosition p[2];
		p[0].position = _light.mPosition.getPosition();
		p[1].position = Vec3::ZERO;

		//
		Euclid::Effect* fx = _material->getEffect();
		if (NULL == fx)
		{
			return;
		}
		_material->apply();
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);
			if (i == 0)
			{
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 2, p, sizeof(Euclid::sPosition));
			}
			fx->endPass();
		}
		fx->end();
	}
	void CViewWindow::_renderModel()
	{
		if (_model)
		{
			_model->render();
		}
	}
	void CViewWindow::_renderFont()
	{
		// 文字最后画
		if (_font[Euclid::eFontProperty_Normal])
		{
			std::ostringstream ss;
			ss<<"FPS = "<<int(_fps);
			_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
		}
	}
	void CViewWindow::_clear()
	{
		_painted = false;
		_material = NULL;
		_vb = NULL;
		for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
		{
			_font[i] = NULL;
		}
		_fps = 0.0f;
		_clearModel();
		//_camera.setPosition(Vec3(0.0f, 50.0f, 150.0f));
	}
	void CViewWindow::_calcFPS()
	{
		static float last_time = (float)timeGetTime();
		float current_time = (float)timeGetTime();
		static int count_frames = 0;
		++count_frames;
		float delta = current_time - last_time;
		// calculate FPS per Second
		if(delta >= 1000.0f)
		{
			_fps = (float)count_frames * 1000.0f  / delta;
			last_time = current_time;
			count_frames = 0;
		}
	}
	void CViewWindow::_clearModel()
	{
		_modelFileName.clear();
		_model = NULL;
	}
	void CViewWindow::_destroymodel()
	{
		if (_model)
		{
			_model->destroy();
			delete _model;
			_model = NULL;
		}
	}


	void CViewWindow::changeModelFile( const std::string& fileName )
	{
		size_t dotPos = fileName.find('.');
		if (dotPos == std::string::npos)
		{
			return;
		}
		std::string suffix = fileName.substr(dotPos, fileName.size() - dotPos);
		std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::tolower);
		EventArgsCTreeDlgChangeFile args;
		if(suffix == ".subentity")
		{
			_destroymodel();
			_clearModel();
			_model = new SubEntity;
			args.mType = eVisibleObjectType_SubEntity;
		}
		else if(suffix == ".entity")
		{
			_destroymodel();
			_clearModel();
			_model = new Entity;
			args.mType = eVisibleObjectType_Entity;
		}
		else if(suffix == ".mz")
		{
			_destroymodel();
			_clearModel();
			_model = new MzModel;
			args.mType = eVisibleObjectType_MzModel;
		}
		else
		{
			return;
		}
		std::string name = Buddha::FileSystem::getInstancePtr()->standardFilePath(fileName);	
		_model->decode(name);
		//
		args.mPrimitive = _model;
		EventManager::getInstancePtr()->fireEvent(eEvent_CTreeDlgChangeFile, &args);
	}

void CViewWindow::onIdle()
{
	if (RenderEngineImp::isNull()) 
	{
		return;
	}
	static u32 oldTime = GetTickCount();
	u32 currentTime = GetTickCount();
	float delta = currentTime - oldTime;
	oldTime = currentTime;
	//_camera.lookAt(Vec3::ZERO);
	Vec3 pos = _cameraRelativeShpereCoordination.getPosition();
	_camera.setPosition(pos);
	float s = Euler::Basic::Sin(_cameraRelativeShpereCoordination.mAngleUpFromXZ);
	//进入球的背面
	Vec3 lookTaTarget = _cameraRelativeShpereCoordination.mOrigion;
	if (s < 0)
	{
		_camera.lookAt(lookTaTarget, Vec3::NEGATIVE_UNIT_Y);
	}
	else
	{
		_camera.lookAt(lookTaTarget);
	}
	_camera.update();
	Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix();
	if (_model)
	{
		_model->setMatrix("gWorldViewProj", m);
		Mat4 w = Mat4::IDENTITY;
		_model->setMatrix("gWorld", w);
		_model->setMatrix("gWorldInverseTranspose", w);
		_model->setMatrix("gView", _camera.getViewMatrix());
		_model->setMatrix("gProjection", _camera.getProjectionMatrix());
		//
		{
			Euclid::Effect* fx = RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->getEffectByFile("shader/aPTSkin.fx");
			if (fx)
			{
				Vec3 l = _light.mPosition.getPosition();
				l.normalise();
				Vec4 p(l.x, l.y, l.z, 0);
				fx->setVector("gLightPosition", &p);
			}
		}
		//
		_model->update(delta);
	}
	Euclid::Effect* fx = _material->getEffect();
	if (fx)
	{
		fx->setMatrix("g_mWorldViewProjection", m);
	}

	//
	{
		if (gTerrain.getMaterial())
		{
			Euclid::Effect* fx = gTerrain.getMaterial()->getEffect();
			if (fx)
			{
				fx->setMatrix("g_mViewProjection", m);
			}
		}
	}
	//
	{
		if (mSculptor.mMaterial)
		{
			Euclid::Effect* fx = mSculptor.mMaterial->getEffect();
			if (fx)
			{
				fx->setMatrix("g_mViewProjection", m);
			}
		}
	}
	_calcFPS();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_render();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);
}
Chunks      gTerrain;