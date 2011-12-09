#include "IMaterial.h"
#include "RenderSystem.h"

namespace Euclid
{
	//
	void IMaterial::setVertexDeclaration(eVertexDeclarationType e)
	{
		_vertexDeclaration = e;
	}
	//
	void IMaterial::apply()
	{
		RenderSystem::getInstancePtr()->setVertexDeclaration(_vertexDeclaration);
	}
}