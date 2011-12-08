//==========================================================================
/**
* @file	  : RenderableObjectFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   20:11
* purpose : 
*/
//==========================================================================

#ifndef __RenderableObjectFactory_h__
#define __RenderableObjectFactory_h__

#include "Common.h"
//
namespace Euclid
{
	class RenderableObjectFactory
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara& para)
		{
			return NULL;
		}

		virtual RenderableObject* createInstanceImpl(RenderablePara* para)
		{
			return NULL;
		}

		virtual RenderableObject* createInstanceImpl(const std::string& name, const std::string& meshName = "")
		{
			return NULL;
		}
	};
}

#endif // __RenderableObjectFactory_h__
 
