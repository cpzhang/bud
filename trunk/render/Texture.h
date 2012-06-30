/********************************************************************
**	file: 		Texture.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-12
**	purpose:	
*********************************************************************/
#ifndef __Texture_h__
#define __Texture_h__


#include "Common.h"
#include "ITexture.h"
//
namespace Euclid
{
	class Texture : public ITexture
	{
		//
	public:
		Texture();
		virtual ~Texture();

		//
	public:
		virtual IDirect3DTexture9*		getTexture();
		virtual bool					createEmpty(u32 Width, u32 Height, u32 Levels, eUsage Usage, eFormat Format, ePool Pool);
		virtual bool					loadFromFile(const tstring& fileName);
		virtual bool					loadFromMemory(unsigned char* buffer, unsigned int length);
		virtual bool					loadFromRawData(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT format);
		virtual bool					setSubData(unsigned int level, unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int pitch, void* pData, D3DFORMAT pf);
		virtual void					destroy();
		virtual void					release();
		virtual void					addReference();
		virtual void onInvalidateDevice();
		virtual void onRestoreDevice();
		virtual bool update(IFlashDXPlayer* p);
		//
	private:
		IDirect3DTexture9*		_texture;
		IDirect3DDevice9*		_device;
		u32						_references;
	};
}


#endif // __Texture_h__