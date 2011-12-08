#include "EntityYZFactory.h"
#include "EntityYZ.h"

namespace Euclid
{
	RenderableObject* EntityYZFactory::createInstanceImpl( RenderablePara* para )
	{
		EntityYZPara* p = static_cast<EntityYZPara*>(para);

		RenderableObject* o = new EntityYZ(p);	
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
