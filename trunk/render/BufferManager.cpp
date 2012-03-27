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

	IBuffer* BufferManager::createVertexBuffer( unsigned int Length, eUsage Usage, ePool Pool )
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
		_buffers.push_back(vb);
		//
		return vb;
	}

	IBuffer* BufferManager::createIndexBuffer( unsigned int Length, eUsage Usage, eFormat Format, ePool Pool )
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
		_buffers.push_back(ib);
		//
		return ib;
	}

	void BufferManager::onInvalidateDevice()
	{
		for (size_t i = 0; i != _buffers.size(); ++i)
		{
			if (_buffers[i])
			{
				_buffers[i]->onInvalidateDevice();
			}
		}
	}

	void BufferManager::onRestoreDevice()
	{
		for (size_t i = 0; i != _buffers.size(); ++i)
		{
			if (_buffers[i])
			{
				_buffers[i]->onRestoreDevice();
			}
		}
	}
}
 
