#include "BufferManager.h"
#include "Buffer.h"
//
namespace Euclid
{
	BufferManager::BufferManager()
	{

	}

	BufferManager::~BufferManager()
	{

	}

	IBuffer* BufferManager::createVertexBuffer( unsigned int Length, unsigned long Usage, ePool Pool )
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

	IBuffer* BufferManager::createIndexBuffer( unsigned int Length, unsigned long Usage, D3DFORMAT Format, ePool Pool )
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
 
