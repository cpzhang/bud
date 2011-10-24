//==========================================================================
/**
* @file	  : EntityYZFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-22   11:53
* purpose : 
*/
//==========================================================================

#ifndef __EntityYZFactory_h__
#define __EntityYZFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"

namespace Euclid
{
	class EntityYZFactory : public RenderableObjectFactory, public Buddha::SingletonEx<EntityYZFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara* para);
	};
}

#endif // __EntityYZFactory_h__
 
