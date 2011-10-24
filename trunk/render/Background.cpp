#include "stdafx.h"
#include "Background.h"
#include "Paras.h"
#include "RenderSystem.h"
#include "TextureFactory.h"
#include "SceneNode.h"
#include "Texture.h"

namespace Euclid
{
	Background::Background(const BackgroundPara& p)
		: _para(p)
	{

	}

	Background::~Background()
	{
		destroy();
	}

	bool Background::create()
	{
		_renderSystem = Paras::getInstancePtr()->_renderSystem;

		D3DVIEWPORT9 vp;
		Paras::getInstancePtr()->_renderSystem->getViewport(&vp);

		_vertex[0] = TPOSITION_TEXTURE(0, 0, 5, 1.0, 0, 0);

		_vertex[1] = TPOSITION_TEXTURE(vp.Width, 0, 5, 1.0, 1, 0);
		
		_vertex[2] = TPOSITION_TEXTURE(vp.Width, vp.Height, 5, 1.0, 1, 1);

		_vertex[3] = TPOSITION_TEXTURE(0, vp.Height, 5, 1.0, 0, 1);

		_tex = TextureFactory::getInstancePtr()->createTextureFromFile(_para._texName);

		if (_tex == NULL)
		{
			Error("Failed to create Background Texture : " + _para._texName);
			return false;
		}
	
		return true;
	}

	void Background::destroy()
	{
		safeDestroy(_tex);
	}

	void Background::update(u32 current, u32 delta)
	{

	}

	void Background::render()
	{
		Paras::getInstancePtr()->_renderSystem->drawQuad(&_vertex[0], _tex);
	}

	void Background::preRender()
	{
		//
		if (_currentNode)
		{
			_renderSystem->setRenderState(D3DRS_ZENABLE, false);

			//_renderSystem->setVertexDeclaration(VD_TPOSITION_TEXTURE);

			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//
			_renderSystem->setRenderState(D3DRS_LIGHTING, false);
			//
			_currentNode->_updateFromParent();
			
			_renderSystem->setWorldMatrix(_currentNode->_getFullTransform());

		}
	}

	void Background::postRender()
	{
		//_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		//
		_renderSystem->setRenderState(D3DRS_ZENABLE, true);

		_renderSystem->setWorldMatrix(Mat4::IDENTITY);

		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);

	}

	Euclid::TPOSITION_TEXTURE Background::_vertex[4];

}
