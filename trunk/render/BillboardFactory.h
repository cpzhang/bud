//==========================================================================
/**
* @file	  : BillboardFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-17   12:09
* purpose : 
*/
//==========================================================================

#ifndef __BillboardFactory_h__
#define __BillboardFactory_h__

#include "Common.h"
#include "RenderableObjectFactory.h"

namespace Euclid
{
	class BillboardFactory : public RenderableObjectFactory, public Buddha::SingletonEx<BillboardFactory>
	{
	public:
		virtual RenderableObject* createInstanceImpl(RenderablePara* para);
	};
}

#endif // __BillboardFactory_h__
 
