#include "SubEntityYZ.h"
#include "IEffect.h"
#include "RenderSystem.h"
#include "EntityYZ.h"

namespace Euclid
{

	SubEntityYZ::SubEntityYZ(EntityYZ* entity)
		:_entity(entity)
	{
		
	}

	SubEntityYZ::~SubEntityYZ()
	{

	}

	void SubEntityYZ::renderNormal()
	{

	}

	bool SubEntityYZ::create()
	{
		return true;
	}

	void SubEntityYZ::destroy()
	{

	}

	void SubEntityYZ::preRender()
	{

	}

	void SubEntityYZ::postRender()
	{

	}

	void SubEntityYZ::update( u32 current, u32 delta )
	{

	}

	void SubEntityYZ::renderWithEffect()
	{
		_renderSystem->setVertexDeclaration(_entity->_vdt);
		//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		HRESULT hr;

		u32 effectNum = 0;
		// Apply the technique contained in the effect 
		_effect->begin(&effectNum);

		for (u32 i = 0; i < effectNum; ++i)
		{
			_effect->beginPass(i);

			renderNormal();

			_effect->endPass();
		}

		_effect->end();
	}

}