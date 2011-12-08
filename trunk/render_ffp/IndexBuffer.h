//==========================================================================
/**
* @file	  : IndexBuffer.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-5   17:43
* purpose : 
*/
//==========================================================================

#ifndef __IndexBuffer_h__
#define __IndexBuffer_h__

#include "stdafx.h"
#include "Common.h"

//
namespace Euclid
{
	class _EuclidExport_ IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		//
	public:
		IDirect3DIndexBuffer9*		getIndexBuffer(); 

		//
		bool create(unsigned int Length, unsigned long Usage, D3DFORMAT Format, D3DPOOL Pool );
		//
		void destroy();
		//
		void* lock(unsigned int OffsetToLock, unsigned int SizeToLock, unsigned long Flags);
		//
		void unLock();
		//
	private:
		IDirect3DIndexBuffer9*		_ib; 
		IDirect3DDevice9*			_device;
		bool						_isLocked;
	};
}


#endif // __IndexBuffer_h__
 
