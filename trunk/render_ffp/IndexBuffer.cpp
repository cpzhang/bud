#include "IndexBuffer.h"
#include "Paras.h"

//
namespace Euclid
{
	IndexBuffer::IndexBuffer()
		:	_ib(0), _device(0), _isLocked(false)
	{
		_device = Paras::getInstancePtr()->_device;
	}

	IndexBuffer::~IndexBuffer()
	{
		destroy();
	}

	IDirect3DIndexBuffer9* IndexBuffer::getIndexBuffer()
	{
		return _ib;
	}

	bool IndexBuffer::create( unsigned int Length, unsigned long Usage, D3DFORMAT Format, D3DPOOL Pool )
	{
		if (!_device)
		{
			return false;
		}

		//
		HRESULT hr;
		hr = _device->CreateIndexBuffer(Length, Usage, Format, Pool, &_ib, NULL);
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
}
