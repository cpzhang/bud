//==========================================================================
/**
* @file	  : EntityXFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   20:15
* purpose : 
*/
//==========================================================================

#ifndef __EntityXFactory_h__
#define __EntityXFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"
//
namespace Euclid
{
	class EntityXFactory : public RenderableObjectFactory, public Buddha::SingletonEx<EntityXFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(const std::string& name, const std::string& meshName = "");
	};
}

#endif // __EntityXFactory_h__
 
