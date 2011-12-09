#include "RenderEngine.h"
#include "RenderSystem.h"
#include "Material.h"
#include "BufferManager.h"
#include "EffectManager.h"
namespace Euclid
{
	bool RenderEngine::create()
	{
		//
		new RenderSystem;
		if (!RenderSystem::getInstancePtr()->create())
		{
			return false;
		}

		//
		new MaterialManager;

		//
		new BufferManager;

		//
		new EffectManager;

		return true;
	}


	bool RenderEngine::destroy()
	{
		//
		delete EffectManager::getInstancePtr();

		//
		delete BufferManager::getInstancePtr();

		//
		delete MaterialManager::getInstancePtr();

		//
		delete RenderEngineCreationParameters::getInstancePtr();

		//
		if (!RenderSystem::getInstancePtr()->destroy())
		{
			return false;
		}
		delete RenderSystem::getInstancePtr();
		return true;
	}

	IRenderSystem* RenderEngine::getRenderSystem()
	{
		return RenderSystem::getInstancePtr();
	}


	RenderEngine::RenderEngine()
	{
		new RenderEngineCreationParameters;
	}

	RenderEngine::~RenderEngine()
	{

	}

	RenderEngineCreationParameters* RenderEngine::getCreationParameters()
	{
		return RenderEngineCreationParameters::getInstancePtr();
	}

	IMaterialManager* RenderEngine::getMaterialManager()
	{
		return MaterialManager::getInstancePtr();
	}

	BufferManager* RenderEngine::getBufferManager()
	{
		return BufferManager::getInstancePtr();
	}

	EffectManager* RenderEngine::getEffectManager()
	{
		return EffectManager::getInstancePtr();
	}

	extern "C" _EuclidExport_ IRenderEngine* APIENTRY createRenderEngine()
	{
		IRenderEngine* renderEngine = new RenderEngine;
		if (NULL == renderEngine)
		{
			return NULL;
		}
		//
		return renderEngine;
	}
}
