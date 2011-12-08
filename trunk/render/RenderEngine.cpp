#include "RenderEngine.h"
#include "RenderSystem.h"

namespace Euclid
{
	bool RenderEngine::create()
	{
		new RenderSystem;
		if (!RenderSystem::getInstancePtr()->create())
		{
			return false;
		}
		return true;
	}


	bool RenderEngine::destroy()
	{
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
