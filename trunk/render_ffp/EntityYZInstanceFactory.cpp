#include "EntityYZInstanceFactory.h"
#include "EntityYZInstance.h"

namespace Euclid
{
	RenderableObject* EntityYZInstanceFactory::createInstanceImpl( RenderablePara* para )
	{
		EntityYZInstancePara* p = static_cast<EntityYZInstancePara*>(para);

		RenderableObject* o = new EntityYZInstance(p);	
		//
		if (o)
		{

			if (o->create())
			{
				return o;
			}
			else
			{
				o->destroy();
			}
		}

		//
		return NULL;		
	}
}