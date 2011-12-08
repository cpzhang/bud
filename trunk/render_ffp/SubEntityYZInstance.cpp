#include "SubEntityYZInstance.h"
#include "IRenderSystem.h"
#include "IEffect.h"
#include "EntityYZ.h"
#include "EntityYZMaterial.h"
#include "TextureFactory.h"
#include "EntityYZInstance.h"

namespace Euclid
{
	SubEntityYZInstance::SubEntityYZInstance(SubEntityYZInstancePara* p)
		:_para(NULL)
	{
		_para = new SubEntityYZInstancePara(p);
	}

	SubEntityYZInstance::~SubEntityYZInstance()
	{

	}

	bool SubEntityYZInstance::create()
	{
		_ib = _para->_entityYZInstance->_ib;
		_vb = _para->_entityYZInstance->_vb;
		if (_vb == NULL)
		{
			return false;
		}

		return true;
	}

	void SubEntityYZInstance::destroy()
	{

	}

	void SubEntityYZInstance::preRender()
	{

	}

	void SubEntityYZInstance::postRender()
	{

	}

	void SubEntityYZInstance::update( u32 current, u32 delta )
	{
		
	}

	void SubEntityYZInstance::renderNormal()
	{
		SubMeshMap::iterator it = _para->_entityYZInstance->_para->_entityYZ->_sm.find(_para->_subEntityName);
		if (it == _para->_entityYZInstance->_para->_entityYZ->_sm.end())
		{
			return;
		}

		ITexture* tex = NULL;
		SUBMESH_BASE* b = it->second->_base;
		if (b && b->matId != _para->_entityYZInstance->_para->_entityYZ->_materials.size())
		{
			EntityYZMaterial* m = _para->_entityYZInstance->_para->_entityYZ->_materials[b->matId];
			if (m && !m->_layers.empty())
			{
				tex = m->getLayer(0)._texture;
				if (tex == NULL)
				{
					tex = TextureFactory::getInstancePtr()->createTextureFromFile(m->getLayer(0)._tex);
					m->getLayer(0)._texture = tex;
				}
			}
		}
		_effect->setTexture("g_MeshTexture", tex);
		if (tex)
		{
			_effect->setInt("g_isTextured", true);
			//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			_effect->setInt("g_isTextured", false);
			_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		_renderSystem->setStreamSource(0, _vb, 0, sizeof(POSITION_TEXTURE));
		_renderSystem->setIndices(_ib);
		_renderSystem->drawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, b->vstart, b->vcount, b->istart, b->icount / 3);
	}

	void SubEntityYZInstance::renderWithEffect()
	{
		_renderSystem->setVertexDeclaration(_vdt);

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