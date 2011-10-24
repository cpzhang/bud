#include "ManualObjectFactory.h"
#include "ManualObject.h"
#include "LightningStrip.h"
#include "Spiral.h"
#include "Blooding.h"
//
namespace Euclid
{
	RenderableObject* ManualObjectFactory::createInstanceImpl( const std::string& name, const std::string& meshName /*= ""*/ )
	{
		//
		RenderableObject* o = new ManualObject(name);

		//
		if (o)
		{
			return o;
// 
// 			if (o->create())
// 			{
// 				return o;
// 			}
// 			else
// 			{
// 				o->destroy();
// 			}
		}

		//
		return NULL;
	}

	RenderableObject* ManualObjectFactory::createInstanceImpl(RenderablePara& para )
	{
		ManualPara* p = static_cast<ManualPara*>(&para);

		//RenderableObject* o = new Blooding(p->_texFile, p->_rotateSpeed);	
		RenderableObject* o = new Blooding(p);	
		//
		if (o)
		{

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
