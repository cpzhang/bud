#include "BillboardFactory.h"
#include "Billboard.h"

namespace Euclid
{
	RenderableObject* BillboardFactory::createInstanceImpl( RenderablePara* para )
	{
		BillboardPara* p = static_cast<BillboardPara*>(para);

		RenderableObject* o = new Billboard(p);	
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
