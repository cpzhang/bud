#include "BackgroundFactory.h"
#include "Background.h"

namespace Euclid
{
	RenderableObject* BackgroundFactory::createInstanceImpl( RenderablePara& para )
	{
		BackgroundPara* p = static_cast<BackgroundPara*>(&para);

		RenderableObject* o = new Background(*p);	
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