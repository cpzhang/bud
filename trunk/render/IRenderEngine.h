/********************************************************************
**	file: 		IRenderEngine.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __IRenderEngine_h__
#define __IRenderEngine_h__

#include "Common.h"

namespace Euclid
{
	class _EuclidExport_ IRenderEngine
	{
	public:
		IRenderEngine()
		{

		}

		virtual ~IRenderEngine()
		{

		}
	public:
		virtual bool				create() = 0;
		virtual bool				destroy() = 0;
		virtual IRenderSystem*		getRenderSystem() = 0;
	};

	// Factory function that creates instances of the IRenderEngine object.
	extern "C" _EuclidExport_ IRenderEngine* APIENTRY createRenderEngine(HWND hwnd);
}

#endif // __IRenderEngine_h__