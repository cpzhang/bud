#include "ParticleSystemFactory.h" 
#include "ParticleSystem.h"

namespace Euclid
{
	RenderableObject* ParticleSystemFactory::createInstanceImpl( RenderablePara* para )
	{
		ParticleSystemPara* p = static_cast<ParticleSystemPara*>(para);

		RenderableObject* o = new ParticleSystem(p);	
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