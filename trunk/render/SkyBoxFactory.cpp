#include "stdafx.h"
#include "SkyBoxFactory.h"
#include "SkyBox.h"
#include "ZIPManager.h"

//
namespace Euclid
{
	RenderableObject* SkyBoxFactory::createInstanceImpl( const std::string& name, const std::string& meshName /*= ""*/ )
	{
		SkyBox* o = new SkyBox;

		//
		if (o)
		{
			ZIPManager::getInstancePtr()->addZIP(meshName);

			if (o->create())
			{
				return o;
			}
			else
			{
				o->destroy();
			}
		}

		//
		return NULL;
	}
}
 
