//==========================================================================
/**
* @file	  : VertexBuffer.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-5   17:37
* purpose : 
*/
//==========================================================================

#ifndef __VertexBuffer_h__
#define __VertexBuffer_h__

#include "Common.h"

//
namespace Euclid
{
	class _EuclidExport_ VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		//
	public:
		IDirect3DVertexBuffer9*		getVertexBuffer(); 
		//
		bool create( unsigned int Length, unsigned long Usage, D3DPOOL Pool );
		//
		void destroy();
		//
		bool isLocked();
		//
		void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags);
		//
		void unLock();
		//
	private:
		IDirect3DVertexBuffer9*		_vb; 
		IDirect3DDevice9*			_device;
		bool						_isLocked;
	};
}

#endif // __VertexBuffer_h__
 
