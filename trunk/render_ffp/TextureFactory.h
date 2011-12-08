//==========================================================================
/**
* @file	  : TextureFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   18:24
* purpose : 
*/
//==========================================================================

#ifndef __TextureFactory_h__
#define __TextureFactory_h__

#include "Common.h"
#include "ITextureFactory.h"
//
namespace Euclid
{
	class TextureFactory : public ITextureFactory, public Buddha::SingletonEx<TextureFactory>
	{
	public:
		TextureFactory();
		~TextureFactory();

		//
	public:
		virtual ITexture*	createEmptyTexture(unsigned int width, unsigned int height, D3DFORMAT pf);
		virtual ITexture*	createTextureFromFile(const std::string& fileName);
		virtual ITexture*	createTexFromMemory(unsigned char* buffer, unsigned int length);
		virtual ITexture*	createTextureFromRawData(unsigned char* pBuffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT pf);
	public:
		//
		void onReleaseTexture(ITexture* tex);
		//
	private:
		std::map<std::string, ITexture*>	_textures;
	};
}

#endif // __TextureFactory_h__
 
