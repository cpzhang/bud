#include "BufferFactory.h"

//
namespace Euclid
{
	BufferFactory::BufferFactory()
	{

	}

	BufferFactory::~BufferFactory()
	{

	}

	VertexBuffer* BufferFactory::createVertexBuffer( unsigned int Length, unsigned long Usage, D3DPOOL Pool )
	{
		VertexBuffer* vb = new VertexBuffer;
		if (vb)
		{
			if (!vb->create(Length, Usage, Pool))
			{
				delete vb;
				vb = NULL;
			}
		}

		//
		return vb;
	}

	IndexBuffer* BufferFactory::createIndexBuffer( unsigned int Length, unsigned long Usage, D3DFORMAT Format, D3DPOOL Pool )
	{
		IndexBuffer* ib = new IndexBuffer;
		if (ib)
		{
			if (!ib->create(Length, Usage, Format, Pool))
			{
				delete ib;
				ib = NULL;
			}
		}

		//
		return ib;
	}
}
 
