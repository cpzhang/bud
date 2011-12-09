#include "Buffer.h"
#include "RenderSystem.h"
//
namespace Euclid
{
	IndexBuffer::IndexBuffer()
		:	_ib(0)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	IDirect3DIndexBuffer9* IndexBuffer::getIndexBuffer()
	{
		return _ib;
	}

	bool IndexBuffer::create( unsigned int Length, unsigned long Usage, D3DFORMAT Format, ePool Pool )
	{
		//
		HRESULT hr;
		hr = RenderSystem::getInstancePtr()->getDevice()->CreateIndexBuffer(Length, Usage, Format, (D3DPOOL)Pool, &_ib, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		//
		return true;
	}

	void* IndexBuffer::lock( unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags )
	{
		void* data = 0;

		//
		if (_ib)
		{
			_ib->Lock(OffsetToLock, SizeToLock, &data, Flags);
		}

		//
		if (data)
		{
			_isLocked = true;
		}

		//
		return data;
	}

	void IndexBuffer::unLock()
	{
		if (_ib)
		{
			//
			_ib->Unlock();

			//
			_isLocked = false;
		}
	}

	void IndexBuffer::destroy()
	{
		if (_ib)
		{
			_ib->Release();
			_ib = 0;
		}
	}

	IBuffer::IBuffer()
		:_isLocked(false)
	{

	}

	//
	VertexBuffer::VertexBuffer()
		:	_vb(0)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	IDirect3DVertexBuffer9* VertexBuffer::getVertexBuffer()
	{
		return _vb;
	}

	bool VertexBuffer::create( unsigned int Length, unsigned long Usage, ePool Pool )
	{
		//
		HRESULT hr;
		// D3DPOOL_DEFAULT flag tells Direct3D to create the vertex buffer in the memory allocation that is most appropriate for this buffer
		hr = RenderSystem::getInstancePtr()->getDevice()->CreateVertexBuffer(Length, Usage, 0, (D3DPOOL)Pool, &_vb, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		//
		return true;
	}

	bool VertexBuffer::isLocked()
	{
		return _isLocked;
	}

	// This mechanism of locking and unlocking is required because the vertex buffer may be in device memory.
	void* VertexBuffer::lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags)
	{
		void* data = NULL;

		//
		if (_vb)
		{
			_vb->Lock(OffsetToLock, SizeToLock, &data, Flags);
		}

		//
		if (data)
		{
			_isLocked = true;
		}

		//
		return data;
	}

	void VertexBuffer::unLock()
	{
		if (_vb)
		{
			//
			_vb->Unlock();

			//
			_isLocked = false;
		}
	}

	void VertexBuffer::destroy()
	{
		if (_vb)
		{
			_vb->Release();
			_vb = 0;
		}
	}
}
