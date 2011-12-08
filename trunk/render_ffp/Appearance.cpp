#include "Appearance.h" 
#include "D3D9Mapping.h"
namespace Euclid
{
	RenderSpace::RenderSpace()
	{
		//
		_components.push_back(new RenderSpaceComponent_RenderState("lighting", D3DRS_LIGHTING, false));	
		_components.push_back(new RenderSpaceComponent_RenderState("fill mode", D3DRS_FILLMODE, D3DFILL_SOLID));
		_components.push_back(new RenderSpaceComponent_RenderState("cull mode", D3DRS_CULLMODE, D3DCULL_NONE));
		_components.push_back(new RenderSpaceComponent_RenderState("alpha blend enable", D3DRS_ALPHABLENDENABLE, false));
		_components.push_back(new RenderSpaceComponent_RenderState("src blend", D3DRS_SRCBLEND, D3DBLEND_ONE));
		_components.push_back(new RenderSpaceComponent_RenderState("dest blend", D3DRS_DESTBLEND, D3DBLEND_ZERO));
		_components.push_back(new RenderSpaceComponent_RenderState("z enable", D3DRS_ZENABLE, D3DZB_TRUE));
		_components.push_back(new RenderSpaceComponent_RenderState("z write enable", D3DRS_ZWRITEENABLE, TRUE));
		_components.push_back(new RenderSpaceComponent_RenderState("alpha test enable", D3DRS_ALPHATESTENABLE, TRUE));
		_components.push_back(new RenderSpaceComponent_RenderState("scissor test enable", D3DRS_SCISSORTESTENABLE, false));
		//
		_components.push_back(new RenderSpaceComponent_VertexDeclaration("vertex declaration", VD_NULL));
		//
		_components.push_back(new RenderSpaceComponent_WorldTransform("world transformation", Mat4::IDENTITY));
		//
		_components.push_back(new RenderSpaceComponent_ViewTransform("view transformation", Mat4::IDENTITY));
		//
		_components.push_back(new RenderSpaceComponent_ProjectTransform("project transformation", Mat4::IDENTITY));
		//
		_components.push_back(new RenderSpaceComponent_VertexBuffer("vertex buffer", NULL, 0, 0));
		//
		_components.push_back(new RenderSpaceComponent_IndexBuffer("index buffer", NULL, 0));
		//
		_components.push_back(new RenderSpaceComponent_Texture("texture", NULL));
	}

	RenderSpace::~RenderSpace()
	{
		STLDeleteSequence(_components);
	}

	std::vector<size_t>& RenderSpace::difference( const RenderSpace& rs )
	{
		static std::vector<size_t> ds;
		ds.clear();
		for (size_t i = 0; i != _components.size(); ++i)
		{
			if (!_components[i]->equal(rs._components[i]))
			{
				ds.push_back(i);
			}
		}

		return ds;
	}

	void RenderSpace::render()
	{
		RenderSpaceComponent* rsc = getComponent("index buffer");
		RenderSpaceComponent_IndexBuffer* rscib = (RenderSpaceComponent_IndexBuffer*)rsc;
		IndexBuffer* ib = rscib->_ib;
		if (ib)
		{
			RenderSpaceComponent* rsc = getComponent("vertex buffer");
			RenderSpaceComponent_VertexBuffer* rscvb = (RenderSpaceComponent_VertexBuffer*)rsc;
			//_renderSystem->setIndices(ib);
			//Paras::getInstancePtr()->_renderSystem->drawIndexedPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, 0, rscvb->_vertexCount, 0, _primitiveCount);
		}
		else
		{
			//
			//_renderSystem->drawPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, _getPrimitiveCount());
			//Paras::getInstancePtr()->_renderSystem->drawPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, _primitiveCount);
		}
	}

	RenderSpaceComponent* Euclid::RenderSpace::getComponent( const std::string& rName )
	{
		RenderSpaceComponent* rsc = NULL;
		for (size_t i = 0; i != _components.size(); ++i)
		{
			if (_components[i]->_name == rName)
			{
				rsc = _components[i];
				break;
			}
		}
		return rsc;
	}

	RenderSpace Euclid::RenderSpace::tZero;
}