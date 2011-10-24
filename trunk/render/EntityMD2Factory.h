//==========================================================================
/**
* @file	  : EntityMD2Factory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-17   14:25
* purpose : 
*/
//==========================================================================

#ifndef __EntityMD2Factory_h__
#define __EntityMD2Factory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"

namespace Euclid
{
	class EntityMD2Factory : public RenderableObjectFactory, public Buddha::SingletonEx<EntityMD2Factory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(const std::string& name, const std::string& meshName = "");
	};
}

#endif // __EntityMD2Factory_h__
 
