//==========================================================================
/**
* @file	  : ITextureFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-1   13:00
* purpose : 
*/
//==========================================================================

#ifndef __ITextureFactory_h__
#define __ITextureFactory_h__
//
#include "stdafx.h"
#include "Common.h"

namespace Euclid
{
	class ITextureFactory
	{
	public:
		virtual ITexture*	createEmptyTexture(unsigned int width, unsigned int height, D3DFORMAT pf) = 0;
		virtual ITexture*	createTextureFromFile(const std::string& fileName) = 0;
		virtual ITexture*	createTexFromMemory(unsigned char* buffer, unsigned int length) = 0;
		virtual ITexture*	createTextureFromRawData(unsigned char* pBuffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT pf) = 0;
	};
}

#endif // __ITextureFactory_h__
 
