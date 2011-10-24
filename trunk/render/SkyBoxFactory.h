//==========================================================================
/**
* @file	  : SkyBoxFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-23   14:46
* purpose : 
*/
//==========================================================================

#ifndef __SkyBoxFactory_h__
#define __SkyBoxFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"
//
namespace Euclid
{
	class SkyBoxFactory : public RenderableObjectFactory, public Buddha::SingletonEx<SkyBoxFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(const std::string& name, const std::string& meshName = "");
	};
}

#endif // __SkyBoxFactory_h__
 
