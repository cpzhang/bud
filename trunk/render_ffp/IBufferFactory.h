//==========================================================================
/**
* @file	  : IBufferFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-5   19:58
* purpose : 
*/
//==========================================================================

#ifndef __IBufferFactory_h__
#define __IBufferFactory_h__

//
#include "Common.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

//
namespace Euclid
{
	class IBufferFactory
	{
	public:
		virtual VertexBuffer*	createVertexBuffer(unsigned int Length,	unsigned long Usage, D3DPOOL Pool) = 0;
		virtual IndexBuffer*	createIndexBuffer(unsigned int Length,	unsigned long Usage, D3DFORMAT Format,	D3DPOOL Pool) = 0;
	};
}

#endif // __IBufferFactory_h__
 
