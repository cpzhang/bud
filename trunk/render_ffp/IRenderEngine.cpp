#include "RenderEngine.h" 
#include "Paras.h"
//
namespace Euclid
{
	extern "C" _EuclidExport_ IRenderEngine* APIENTRY createRenderEngine(HWND hwnd)
	{
		IRenderEngine* renderEngine = new RenderEngine;
		if (NULL == renderEngine)
		{
			return NULL;
		}

		//
		new Paras;

		//
		Paras::getInstancePtr()->_hwnd = hwnd;

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