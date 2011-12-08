//==========================================================================
/**
* @file	  : RenderEngine.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   21:30
* purpose : 
*/
//==========================================================================

#ifndef __RenderEngine_h__
#define __RenderEngine_h__

#include "Common.h"
#include "IRenderEngine.h"

//
namespace Euclid
{
	class RenderEngine : public Euclid::IRenderEngine
	{
	public:
		RenderEngine();
		~RenderEngine();

		//
	public:
		virtual bool				create();
		virtual IRenderSystem*		getRenderSystem();
		virtual IFontFactory*		getFontFactory();
		virtual ITextureFactory*	getTextureFactory();
		virtual IBufferFactory*		getBufferFactory();
		virtual ISceneManager*		getSceneManager();
		virtual IZIPManager*		getZIPManager();
		//virtual Camera*				getCamera();
		virtual CameraEx*				getCameraEx();
		virtual void				destroy();
		virtual IShaderFactory*		getShaderFactory();
		virtual IEffectFactory*		getEffectFactory();

		//
	private:
		//
		IRenderSystem*	_renderSystem;

		//
		IFontFactory*	_fontFactory;
	};
}

#endif // __RenderEngine_h__
 
