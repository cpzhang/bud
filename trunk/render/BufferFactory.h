//==========================================================================
/**
* @file	  : BufferFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-5   20:05
* purpose : 
*/
//==========================================================================

#ifndef __BufferFactory_h__
#define __BufferFactory_h__

//
#include "Common.h"
#include "IBufferFactory.h"
//
namespace Euclid
{
	class BufferFactory : public IBufferFactory, public Buddha::SingletonEx<BufferFactory>
	{
	public:
		BufferFactory();
		~BufferFactory();

		//
	public:
	public:
		virtual VertexBuffer*	createVertexBuffer(unsigned int Length,	unsigned long Usage, D3DPOOL Pool);
		virtual IndexBuffer*	createIndexBuffer(unsigned int Length,	unsigned long Usage, D3DFORMAT Format,	D3DPOOL Pool);
	};
}

#endif // __BufferFactory_h__
 
