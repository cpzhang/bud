/********************************************************************
**	file: 		TextureManager.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-12
**	purpose:	
*********************************************************************/
#ifndef __TextureManager_h__
#define __TextureManager_h__

#include "Common.h"
#include "ITextureManager.h"
//
namespace Euclid
{
	class TextureManager : public ITextureManager, public Buddha::SingletonEx<TextureManager>
	{
	public:
		TextureManager();
		~TextureManager();

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

#endif // __TextureManager_h__
