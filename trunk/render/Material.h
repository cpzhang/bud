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

	class MaterialManager: public IMaterialManager, public Buddha::SingletonEx<MaterialManager>
	{
		//
	public:
		virtual IMaterial* createMaterial(eMaterialType e);
	};
}


#endif // __Material_h__