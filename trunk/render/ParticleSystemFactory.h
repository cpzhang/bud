//==========================================================================
/**
* @file	  : ParticleSystemFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-19   16:32
* purpose : 
*/
//==========================================================================

#ifndef __ParticleSystemFactory_h__
#define __ParticleSystemFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"

namespace Euclid
{
	class ParticleSystemFactory : public RenderableObjectFactory, public Buddha::SingletonEx<ParticleSystemFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara* para);
	};
}

#endif // __ParticleSystemFactory_h__
 
