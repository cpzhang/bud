/********************************************************************
**	file: 		Buffer.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __Buffer_h__
#define __Buffer_h__
#include "Common.h"
#include "external/directx/Include/d3d9.h"
#include "RenderEngineCreationParameters.h"
namespace Euclid
{
	class IBuffer
	{
	public:
		//
		virtual IDirect3DIndexBuffer9*		getIndexBuffer() {return NULL;}
		virtual IDirect3DVertexBuffer9*		getVertexBuffer() {return NULL;}
		//
		virtual bool create( unsigned int Length, unsigned long Usage, ePool Pool ) {return true;}
		virtual bool create(unsigned int Length, unsigned long Usage, D3DFORMAT Format, ePool Pool ) {return true;}
		//
		virtual void destroy() = 0;
		//
		virtual void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags) = 0;
		//
		virtual void unLock() = 0;

	public:
		IBuffer();
	protected:
		bool						_isLocked;
	};
	class IndexBuffer : public IBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		//
	public:
		virtual IDirect3DIndexBuffer9*		getIndexBuffer(); 

		//
		virtual bool create(unsigned int Length, unsigned long Usage, D3DFORMAT Format, ePool Pool );
		//
		virtual void destroy();
		//
		virtual void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags);
		//
		virtual void unLock();
		//
	private:
		IDirect3DIndexBuffer9*		_ib; 
	};

	class VertexBuffer : public IBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		//
	public:
		virtual IDirect3DVertexBuffer9*		getVertexBuffer(); 
		//
		virtual bool create( unsigned int Length, unsigned long Usage, ePool Pool );
		//
		virtual void destroy();
		//
		virtual bool isLocked();
		//
		virtual void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags);
		//
		virtual void unLock();
		//
	private:
		IDirect3DVertexBuffer9*		_vb; 
	};
}

#endif // __Buffer_h__