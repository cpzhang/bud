#include "Material.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ITexture.h"
#include "Effect.h"
namespace Zen
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
	{
		setVertexDeclaration(eVertexDeclarationType_PositionTexture);
	}

	void MaterialVertexTexture::apply()
	{
		IMaterial::apply();
		if (NULL == _effect)
		{
			return;
		}
		for (size_t i = 0; i != _textures.size(); ++i)
		{
			_effect->setTexture(_textures[i].first, _textures[i].second);
		}
	}

	bool MaterialVertexTexture::setTexture( const std::string& name, const std::string& fileName )
	{
		ITexture* tex = TextureManager::getInstancePtr()->createTextureFromFile(fileName);
		if (NULL == tex)
		{
			return false;
		}
		_textures.push_back(std::make_pair(name, tex));
		return true;
	}

	void MaterialVertexTexture::destroy()
	{
		IMaterial::destroy();
		for (size_t i = 0; i != _textures.size(); ++i)
		{
			ITexture* tex = _textures[i].second;
			if (tex)
			{
				tex->release();
			}
		}
		_textures.clear();
	}


	u32 MaterialVertexTexture::getStride()
	{
		return sizeof(Zen::sPositionTexture);
	}


	MaterialVertex::MaterialVertex()
	{
		setVertexDeclaration(eVertexDeclarationType_Position);
	}

	u32 MaterialVertex::getStride()
	{
		return sizeof(Zen::sPosition);
	}

}