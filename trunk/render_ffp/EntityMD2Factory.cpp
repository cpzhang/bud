#include "EntityMD2Factory.h"
#include "EntityMD2.h"

namespace Euclid
{
	RenderableObject* EntityMD2Factory::createInstanceImpl( const std::string& name, const std::string& meshName /*= ""*/ )
	{
		EntityMD2* o = new EntityMD2;

		//
		if (o && o->create())
		{
			assert(o->load(meshName));
			o->setName(name);
		}

		//
		return o;
	}
}
