//==========================================================================
/**
* @file	  : BackgroundFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-13   19:18
* purpose : 
*/
//==========================================================================

#ifndef __BackgroundFactory_h__
#define __BackgroundFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"
namespace Euclid
{
	class BackgroundFactory : public RenderableObjectFactory, public Buddha::SingletonEx<BackgroundFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara& para);
	};
}

#endif // __BackgroundFactory_h__
 
