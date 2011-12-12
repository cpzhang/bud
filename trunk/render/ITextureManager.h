/********************************************************************
**	file: 		ITextureManager.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-12
**	purpose:	
*********************************************************************/
#ifndef __ITextureManager_h__
#define __ITextureManager_h__

#include "Common.h"

namespace Euclid
{
	class ITextureManager
	{
	public:
		virtual ITexture*	createEmptyTexture(unsigned int width, unsigned int height, D3DFORMAT pf) = 0;
		virtual ITexture*	createTextureFromFile(const std::string& fileName) = 0;
		virtual ITexture*	createTexFromMemory(unsigned char* buffer, unsigned int length) = 0;
		virtual ITexture*	createTextureFromRawData(unsigned char* pBuffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT pf) = 0;
	};
}

#endif // __ITextureManager_h__
