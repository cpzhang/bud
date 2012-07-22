/********************************************************************
**	file: 		IRenderEngine.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __IRenderEngine_h__
#define __IRenderEngine_h__

#include "Common.h"
#include "RenderEngineCreationParameters.h"
namespace Zen
{
	class _ZenExport_ IRenderEngine
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
		virtual RenderEngineCreationParameters* getCreationParameters() = 0;
		virtual IMaterialManager* getMaterialManager() = 0;
		virtual BufferManager* getBufferManager() = 0;
		virtual EffectManager* getEffectManager() = 0;
		virtual ITextureManager* getTextureManager() = 0;
		virtual FontManager* getFontManager() = 0;
	};

	// Factory function that creates instances of the IRenderEngine object.
	extern "C" _ZenExport_ IRenderEngine* APIENTRY createRenderEngine();
}

#endif // __IRenderEngine_h__