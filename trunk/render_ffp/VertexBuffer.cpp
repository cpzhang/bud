#include "stdafx.h"
#include "VertexBuffer.h"
#include "Paras.h"
//
namespace Euclid
{
	VertexBuffer::VertexBuffer()
		:	_vb(0), _device(0), _isLocked(false)
	{
		_device = Paras::getInstancePtr()->_device;
	}

	VertexBuffer::~VertexBuffer()
	{
		destroy();
	}

	IDirect3DVertexBuffer9* VertexBuffer::getVertexBuffer()
	{
		return _vb;
	}

	bool VertexBuffer::create( unsigned int Length, unsigned long Usage, D3DPOOL Pool )
	{
		if (!_device)
		{
			return false;
		}

		//
		HRESULT hr;
		// D3DPOOL_DEFAULT flag tells Direct3D to create the vertex buffer in the memory allocation that is most appropriate for this buffer

		hr = _device->CreateVertexBuffer(Length, Usage, 0, Pool, &_vb, NULL);
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
