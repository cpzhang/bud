/********************************************************************
**	file: 		Material.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __Material_h__
#define __Material_h__
#include "IMaterial.h"
#include "base\Buddha.h"
namespace Zen
{
	class MaterialVertex: public IMaterial
	{
		//
	public:
		MaterialVertex();
		virtual u32 getStride();
	};

	class _ZenExport_ MaterialVertexTexture: public IMaterial
	{
	public:
		virtual void apply();
		//
		virtual void destroy();
		virtual u32 getStride();
		virtual bool setTexture(const std::string& name, const std::string& fileName);
		//
	public:
		MaterialVertexTexture();
	public:
		typedef std::pair<std::string, ITexture*> TexturePair;
		typedef std::vector<TexturePair> TexturePairVec;
		TexturePairVec _textures;
	};

	class MaterialManager: public IMaterialManager, public Zen::SingletonEx<MaterialManager>
	{
		//
	public:
		virtual IMaterial* createMaterial(eMaterialType e);
	};
}


#endif // __Material_h__