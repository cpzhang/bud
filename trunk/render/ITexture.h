/********************************************************************
**	file: 		ITexture.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-12
**	purpose:	
*********************************************************************/
#ifndef __ITexture_h__
#define __ITexture_h__

#include "Common.h"
namespace Euclid
{
	class ITexture
	{
	public:
		ITexture()
		{

		}

		virtual ~ITexture()
		{

		}
	public:
		virtual IDirect3DTexture9*		getTexture()	= 0;
		virtual bool					loadFromFile(const std::string& fileName)	= 0;
		virtual bool					loadFromMemory(unsigned char* buffer, unsigned int length) = 0;
		virtual bool					loadFromRawData(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT format) = 0;
		virtual bool					setSubData(unsigned int level, unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int pitch, void* pData, D3DFORMAT pf) = 0;
		virtual void					destroy() = 0;
		virtual void					addReference() = 0;
		virtual void					release() = 0;
		virtual void onInvalidateDevice() = 0;
		virtual void onRestoreDevice() = 0;
	};
}


#endif // __ITexture_h__