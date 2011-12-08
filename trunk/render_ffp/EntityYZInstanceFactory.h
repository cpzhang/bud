/********************************************************************
**	file: 		EntityYZInstanceFactory.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-6
**	purpose:	
*********************************************************************/
#ifndef __EntityYZInstanceFactory_h__
#define __EntityYZInstanceFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"

namespace Euclid
{
	class EntityYZInstanceFactory : public RenderableObjectFactory, public Buddha::SingletonEx<EntityYZInstanceFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara* para);
	};
}

#endif // __EntityYZInstanceFactory_h__
