/********************************************************************
**	file: 		Buffer.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __Buffer_h__
#define __Buffer_h__
#include "Common.h"
#include "RenderEngineCreationParameters.h"
namespace Zen
{
	class EBuffer : public Zen::Exception
	{
	public:
		/** Constructor
			* You can provide an internet address. If so, reason *must* contain "%s"
			* where the address should be written. Moreover, the length of reason plus
			* the length of the address when displayed by asString() should no exceed 256.
			*/
		EBuffer( HRESULT r)
		{
			std::ostringstream ss;
			ss<<"Buffer error, string: "<<DXGetErrorString(r)<<";description: "<<DXGetErrorDescription(r);
			_reason = ss.str();
		}
	};
	class EReleaseLeak : public Zen::Exception
	{
	public:
		/** Constructor
			* You can provide an internet address. If so, reason *must* contain "%s"
			* where the address should be written. Moreover, the length of reason plus
			* the length of the address when displayed by asString() should no exceed 256.
			*/
		EReleaseLeak( int r)
		{
			std::ostringstream ss;
			ss<<"release leak: "<<r;
			_reason = ss.str();
		}
	};
	class IBuffer
	{
	public:
		//
		virtual IDirect3DIndexBuffer9*		getIndexBuffer() {return NULL;}
		virtual IDirect3DVertexBuffer9*		getVertexBuffer() {return NULL;}
		//
		virtual bool create( unsigned int Length, unsigned long Usage, ePool Pool ) {return true;}
		virtual bool create(unsigned int Length, unsigned long Usage, eFormat Format, ePool Pool ) {return true;}
		//
		virtual void destroy() = 0;
		//
		virtual void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags) = 0;
		//
		virtual void unLock() = 0;

		virtual void onInvalidateDevice() = 0;
		virtual void onRestoreDevice() = 0;

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
		virtual bool create(unsigned int Length, unsigned long Usage, eFormat Format, ePool Pool );
		//
		virtual void destroy();
		//
		virtual void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags);
		//
		virtual void unLock();
		virtual void onInvalidateDevice();
		virtual void onRestoreDevice();

		//
	private:
		IDirect3DIndexBuffer9*		_ib; 
		unsigned int _Length;
		unsigned long _Usage;
		eFormat _Format;
		ePool _Pool;
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
		virtual void onInvalidateDevice();
		virtual void onRestoreDevice();
		//
	private:
		IDirect3DVertexBuffer9*		_vb; 
		unsigned int _Length;
		unsigned long _Usage;
		ePool _Pool;
	};
}

#endif // __Buffer_h__