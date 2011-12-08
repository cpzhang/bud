/********************************************************************
**	file: 		RenderEngine.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __RenderEngine_h__
#define __RenderEngine_h__

#include "IRenderEngine.h"


namespace Euclid
{
	class RenderEngine: public IRenderEngine
	{
		// IRenderEngine
	public:
		virtual bool				create();
		virtual bool				destroy();
		virtual IRenderSystem*		getRenderSystem();

	public:
		RenderEngine();
		~RenderEngine();
	};
}
#endif // __RenderEngine_h__