#include "Material.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ITexture.h"
namespace Euclid
{
	IMaterial* MaterialManager::createMaterial( eMaterialType e )
	{
		IMaterial* m = NULL;
		switch(e)
		{
		case eMaterialType_Vertex:
			m = new MaterialVertex;
			break;
		case eMaterialType_VertexTexture:
			m = new MaterialVertexTexture;
			break;
		default:
			break;
		}

		return m;
	}

	MaterialVertexTexture::MaterialVertexTexture()
		:_texture(NULL), _lightmapping(NULL)
	{

	}

	void MaterialVertexTexture::apply()
	{
		IMaterial::apply();
		//RenderSystem::getInstancePtr()->setTexture(0, _texture);
	}

	bool MaterialVertexTexture::setTexture( const std::string& fileName )
	{
		_textureFileName = fileName;
		_texture = TextureManager::getInstancePtr()->createTextureFromFile(_textureFileName);
		if (NULL == _texture)
		{
			return false;
		}

		return true;
	}

	void MaterialVertexTexture::destroy()
	{
		if (_texture)
		{
			_texture->release();
			_texture = NULL;
		}
		//
		if (_lightmapping)
		{
			_lightmapping->release();
			_lightmapping = NULL;
		}
	}

	bool MaterialVertexTexture::setLightmapping( const std::string& fileName )
	{
		_lightmapping = TextureManager::getInstancePtr()->createTextureFromFile(fileName);
		if (NULL == _lightmapping)
		{
			return false;
		}

		return true;
	}

}