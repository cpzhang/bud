#include "stdafx.h"
#include "RenderEngine.h" 
#include "Paras.h"
#include "BufferFactory.h"
#include "FontFactory.h"
#include "TextureFactory.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "ZIPManager.h"
#include "Camera.h"
#include "CameraEx.h"
#include "ShaderFactory.h"
#include "EffectFactory.h"

//
namespace Euclid
{
	RenderEngine::RenderEngine()
		: _renderSystem(0), _fontFactory(0)
	{

	}

	RenderEngine::~RenderEngine()
	{

	}

	IRenderSystem* RenderEngine::getRenderSystem()
	{
		return _renderSystem;
	}

	bool RenderEngine::create()
	{
		//
		_renderSystem = new RenderSystem;
		if (NULL == _renderSystem)
		{
			return false;
		}

		//
		if (!_renderSystem->create())
		{
			return false;
		}
		
		//
		Paras::getInstancePtr()->_renderSystem = _renderSystem;

		//
		//Paras::getInstancePtr()->_camera = new Camera;

		//
		Paras::getInstancePtr()->_cameraEx = new CameraEx;

		//
		new SceneManager;
		if (!SceneManager::getInstancePtr()->create())
		{
			return false;
		}

		//
		new FontFactory;

		//
		new TextureFactory;

		//
		new BufferFactory;

		//
		new ZIPManager;

		//
		new ShaderFactory;

		//
		new EffectFactory;

		//
		return true;
	}

	void RenderEngine::destroy()
	{
		//
		if (SceneManager::getInstancePtr())
		{
			SceneManager::getInstancePtr()->destroy();
			delete SceneManager::getInstancePtr();
		}

		//
		if (_renderSystem)
		{
			_renderSystem->destroy();
			delete _renderSystem;
			_renderSystem = NULL;
		}

		//
		if (FontFactory::getInstancePtr())
		{
			delete FontFactory::getInstancePtr();
		}

		//
		if (TextureFactory::getInstancePtr())
		{
			delete TextureFactory::getInstancePtr();
		}

		//
		if (BufferFactory::getInstancePtr())
		{
			delete BufferFactory::getInstancePtr();
		}

		//
		if (ZIPManager::getInstancePtr())
		{
			delete ZIPManager::getInstancePtr();
		}

		//
// 		if (Paras::getInstancePtr()->_camera)
// 		{
// 			delete Paras::getInstancePtr()->_camera;
// 		}

		//
		if (Paras::getInstancePtr()->_cameraEx)
		{
			delete Paras::getInstancePtr()->_cameraEx;
		}

		//
		if (Paras::getInstancePtr())
		{
			delete Paras::getInstancePtr();
		}

		//
		if (ShaderFactory::getInstancePtr())
		{
			delete ShaderFactory::getInstancePtr();
		}

		//
		if (EffectFactory::getInstancePtr())
		{
			delete EffectFactory::getInstancePtr();
		}
	}

	IFontFactory* RenderEngine::getFontFactory()
	{
		return FontFactory::getInstancePtr();
	}

	ITextureFactory* RenderEngine::getTextureFactory()
	{
		return TextureFactory::getInstancePtr();
	}

	IBufferFactory* RenderEngine::getBufferFactory()
	{
		return BufferFactory::getInstancePtr();
	}

	ISceneManager* RenderEngine::getSceneManager()
	{
		return SceneManager::getInstancePtr();
	}

	IZIPManager* RenderEngine::getZIPManager()
	{
		return ZIPManager::getInstancePtr();
	}

// 	Camera* RenderEngine::getCamera()
// 	{
// 		return Paras::getInstancePtr()->_camera;
// 	}


	CameraEx* RenderEngine::getCameraEx()
	{
		return Paras::getInstancePtr()->_cameraEx;
	}

	IShaderFactory* RenderEngine::getShaderFactory()
	{
		return ShaderFactory::getInstancePtr();
	}

	IEffectFactory* RenderEngine::getEffectFactory()
	{
		return EffectFactory::getInstancePtr();
	}

}