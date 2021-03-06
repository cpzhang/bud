#include "stdafx.h"
#include "TextureFactory.h"
#include "Texture.h"
//
namespace Euclid
{
	TextureFactory::TextureFactory()
	{

	}

	TextureFactory::~TextureFactory()
	{

	}

	ITexture* TextureFactory::createEmptyTexture( unsigned int width, unsigned int height, D3DFORMAT pf )
	{
		Texture* tex = new Texture;
		if (NULL == tex)
		{
			return NULL;
		}

		//
		tex->createEmpty(width, height, pf);

		//
		return tex;
	}

	ITexture* TextureFactory::createTextureFromFile(const std::string& fileName )
	{
		std::map<std::string, ITexture*>::iterator it = _textures.find(fileName);
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

	ITexture* TextureFactory::createTexFromMemory( unsigned char* buffer, unsigned int length )
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

	ITexture* TextureFactory::createTextureFromRawData( unsigned char* pBuffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT pf )
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

	void TextureFactory::onReleaseTexture( ITexture* tex )
	{
		std::map<std::string, ITexture*>::iterator it = _textures.begin();
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
