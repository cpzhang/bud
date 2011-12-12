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
namespace Euclid
{
	class MaterialVertex: public IMaterial
	{
		//
	public:
	};

	class _EuclidExport_ MaterialVertexTexture: public IMaterial
	{
	public:
		virtual void apply();
		//
		virtual void destroy();
		//
	public:
		MaterialVertexTexture();

		bool setTexture(const std::string& fileName);
	public:
		std::string _textureFileName;
		ITexture*    _texture;
	};

	class MaterialManager: public IMaterialManager, public Buddha::SingletonEx<MaterialManager>
	{
		//
	public:
		virtual IMaterial* createMaterial(eMaterialType e);
	};
}


#endif // __Material_h__