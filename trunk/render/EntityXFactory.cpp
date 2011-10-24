#include "EntityXFactory.h"
#include "EntityX.h"
//
namespace Euclid
{
	RenderableObject* EntityXFactory::createInstanceImpl( const std::string& name, const std::string& meshName /*= ""*/ )
	{
		EntityX* o = new EntityX;

		//
		if (o && o->create())
		{
			if (!o->loadMeshFromX(meshName))
			{
				return NULL;
			}

			o->setName(name);
		}

		//
		return o;
	}
}
