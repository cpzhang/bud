#include "RenderEngine.h"

namespace Euclid
{
	bool RenderEngine::create()
	{
		return true;
	}


	bool RenderEngine::destroy()
	{
		return true;
	}

	IRenderSystem* RenderEngine::getRenderSystem()
	{
		return 0;
	}


	RenderEngine::RenderEngine()
	{

	}

	RenderEngine::~RenderEngine()
	{

	}

	extern "C" _EuclidExport_ IRenderEngine* APIENTRY createRenderEngine(HWND hwnd)
	{
		IRenderEngine* renderEngine = new RenderEngine;
		if (NULL == renderEngine)
		{
			return NULL;
		}

		//
		if (!renderEngine->create())
		{
			Error("Failed to create RenderEngine!");
			return NULL;
		}

		//
		return renderEngine;
	}
}
