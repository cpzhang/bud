#include "TextureManager.h"
#include "Texture.h"
//
namespace Euclid
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	ITexture* TextureManager::createEmptyTexture(u32 Width, u32 Height, u32 Levels, eUsage Usage, eFormat Format, ePool Pool)
	{
		Texture* tex = new Texture;
		if (NULL == tex)
		{
			return NULL;
		}

		//
		tex->createEmpty(Width, Height, Levels, Usage, Format, Pool);

		//
		return tex;
	}

	ITexture* TextureManager::createTextureFromFile(const tstring& fileName )
	{
		std::map<tstring, ITexture*>::iterator it = _textures.find(fileName);
		if (it != _textures.end())
		{
			it->second->addReference();
			return it->second;
		}
		
		Texture* tex = new Texture;
		if (NULL == tex)
		{
			return NULL;
		}

		//
		if (!tex->loadFromFile(fileName))
		{
			return NULL;
		}

		tex->addReference();
		_textures[fileName] = tex;
		//
		return tex;
	}

	ITexture* TextureManager::createTexFromMemory( unsigned char* buffer, unsigned int length )
	{
		Texture* tex = new Texture;
		if (tex)
		{
			if (!tex->loadFromMemory(buffer, length))
			{
				return NULL;
			}
		}
		return tex;
	}

	ITexture* TextureManager::createTextureFromRawData( unsigned char* pBuffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT pf )
	{
		Texture* tex = new Texture;
		if (tex)
		{
			if (!tex->loadFromRawData(pBuffer, width, height, pitch, pf))
			{
				return NULL;
			}
		}
		return tex;
	}

	void TextureManager::onReleaseTexture( ITexture* tex )
	{
		std::map<tstring, ITexture*>::iterator it = _textures.begin();
		for(; it != _textures.end(); ++it)
		{
			if (tex == it->second)
			{
				_textures.erase(it);
				return;
			}
		}
	}
}
