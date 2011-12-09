#include "Material.h"
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
		default:
			break;
		}

		return m;
	}
}