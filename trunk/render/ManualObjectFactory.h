//==========================================================================
/**
* @file	  : ManualObjectFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-8   11:46
* purpose : 
*/
//==========================================================================

#ifndef __ManualObjectFactory_h__
#define __ManualObjectFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"
//
namespace Euclid
{
	class ManualObjectFactory : public RenderableObjectFactory, public Buddha::SingletonEx<ManualObjectFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara& para);

		virtual RenderableObject* createInstanceImpl(const std::string& name, const std::string& meshName = "");
	};
}

#endif // __ManualObjectFactory_h__
 
