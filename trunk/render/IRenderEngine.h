//==========================================================================
/**
* @file	  : IRenderEngine.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   16:49
* purpose : 
*/
//==========================================================================

#ifndef __IRenderEngine_h__
#define __IRenderEngine_h__
//
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
		virtual IRenderSystem*		getRenderSystem() = 0;
		virtual IFontFactory*		getFontFactory() = 0;
		virtual ITextureFactory*	getTextureFactory() = 0;
		virtual IBufferFactory*		getBufferFactory() = 0;
		virtual ISceneManager*		getSceneManager() = 0;
		virtual IZIPManager*		getZIPManager() = 0;
		//virtual Camera*				getCamera() = 0;
		virtual CameraEx*			getCameraEx() = 0;
		virtual void				destroy() = 0;
		virtual IShaderFactory*		getShaderFactory() = NULL;
		virtual IEffectFactory*		getEffectFactory() = NULL;
	};

	// Factory function that creates instances of the IRenderEngine object.
	extern "C" _EuclidExport_ IRenderEngine* APIENTRY createRenderEngine(HWND hwnd);
}

#endif // __IRenderEngine_h__
