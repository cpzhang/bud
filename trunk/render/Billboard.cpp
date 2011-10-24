#include "stdafx.h"
#include "Billboard.h"
#include "TextureFactory.h"
#include "BufferFactory.h"
#include "VertexDeclaration.h"
#include "Paras.h"
#include "Camera.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "D3D9Mapping.h"

namespace Euclid
{
	Billboard::Billboard( BillboardPara* para )
		:	_vb(NULL), _ib(NULL), _tex(NULL), _normal(Vec3::NEGATIVE_UNIT_Z), _up(Vec3::UNIT_Y)
	{
		_para = new BillboardPara(para);
		//memcpy(_para, para, sizeof(BillboardPara));

//		_camera = Paras::getInstancePtr()->_camera;
	}
	Billboard::~Billboard()
	{
		destroy();
	}

	void Billboard::setCenter( Vec3 center )
	{
		_para->_center = center;
	}

	Vec3 Billboard::getCenter() const
	{
		return _para->_center;
	}

	void Billboard::setOffset( Real offset )
	{
		_para->_offset = offset;
	}

	Real Billboard::getOffset() const
	{
		return _para->_offset;
	}

	void Billboard::update(u32 current, u32 delta)
	{
		if (!_para->_update)
		{
			return;
		}
		POSITION_COLOR_TEXTURE* v = NULL;
		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
		if (v == NULL)
		{
			Error("Failed to Lock the Vertex Buffer");
			return;
		}
/*
0 __ 1
 |\ |
 | \|
3 -- 2
*/
		// 
		_normal = _camera->getEye() - _para->_center;
		_normal.normalise();

		_para->_up.normalise();
		
		Vec3 right = _normal.crossProduct(_para->_up);
		right.normalise();

		v[0]._pos = _para->_center + _para->_offset * _para->_up - _para->_offset * right;
		
		v[1]._pos = _para->_center + _para->_offset * _para->_up + _para->_offset * right;

		v[2]._pos = _para->_center - _para->_offset * _para->_up + _para->_offset * right;

		v[3]._pos = _para->_center - _para->_offset * _para->_up - _para->_offset * right;

		v[0]._color = _para->_color;

		v[1]._color = _para->_color;

		v[2]._color = _para->_color;

		v[3]._color = _para->_color;

		_vb->unLock();
	
	}

	void Billboard::render()
	{
		static unsigned short i[6] = {0, 1, 2, 0, 2, 3};
		
		_renderSystem->setStreamSource(0, _vb, 0, sizeof(POSITION_COLOR_TEXTURE));
		_renderSystem->setIndices(_ib);
		_renderSystem->drawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	}

	bool Billboard::create()
	{
		/*_tex = TextureFactory::getInstancePtr()->createTextureFromFile(_para->_texName);
		if (_tex == NULL)
		{
			Error("Failed to create Texture : " + _para->_texName);
			return false;
		}*/
		
		_vb = BufferFactory::getInstancePtr()->createVertexBuffer(_VertexCount * sizeof(POSITION_COLOR_TEXTURE),
			D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT); 

		if (_vb == NULL)
		{
			Error("Failed to create Vertex Buffer");
			return false;
		}

		POSITION_COLOR_TEXTURE* v = NULL;
		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
		if (v == NULL)
		{
			Error("Failed to Lock the Vertex Buffer");
			return false;
		}
/*
0 __ 1
 |\ |
 | \|
3 -- 2
*/
		// 初始化贴图坐标和颜色
		v[0]._tex = Vec2(0.0f, 0.0f);
		v[0]._color = _para->_color;

		v[1]._tex = Vec2(1.0f, 0.0f);
		v[1]._color = _para->_color;
		
		v[2]._tex = Vec2(1.0f, 1.0f);
		v[2]._color = _para->_color;
		
		v[3]._tex = Vec2(0.0f, 1.0f);
		v[3]._color = _para->_color;

		// 
		_normal = _camera->getEye() - _para->_center;
		_normal.normalise();

		_para->_up.normalise();

		Vec3 right = _normal.crossProduct(_para->_up);
		right.normalise();

		v[0]._pos = _para->_center + _para->_offset * _para->_up - _para->_offset * right;

		v[1]._pos = _para->_center + _para->_offset * _para->_up + _para->_offset * right;

		v[2]._pos = _para->_center - _para->_offset * _para->_up + _para->_offset * right;

		v[3]._pos = _para->_center - _para->_offset * _para->_up - _para->_offset * right;

		_vb->unLock();

		_ib = BufferFactory::getInstancePtr()->createIndexBuffer(6*sizeof(u16), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT);
		if (_ib == NULL)
		{
			Error("Failed to create Index Buffer");
			return false;
		}
		u16* i = (u16*)_ib->lock(0, 0, 0);
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;

		i[3] = 0;
		i[4] = 2;
		i[5] = 3;
		_ib->unLock();

		return true;
	}

	void Billboard::destroy()
	{
		safeDelete(_para);
		safeDestroy(_tex);
		safeDestroy(_vb);
		safeDestroy(_ib);
	}

	void Billboard::preRender()
	{
		if (_currentNode)
		{
			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//
			_renderSystem->setRenderState(D3DRS_LIGHTING, false);
			//
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);

			_renderSystem->setVertexDeclaration(VD_POSITION_COLOR_TEXTURE);
			_renderSystem->setTexture(0, _tex);

			_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
			_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
		}
	}

	void Billboard::postRender()
	{
		_renderSystem->setVertexDeclaration(VD_NULL);
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void Billboard::setColor( RGBA color )
	{
		_para->_color = color;
	}
	const int Billboard::_VertexCount(4);


	BillboardPara::BillboardPara( BillboardPara* bp )
	{
		if (bp != NULL)
		{
			_center = bp->_center;
			_color = bp->_color;
			_offset = bp->_offset;
			_texName = bp->_texName;
			_up = bp->_up;
			_update = bp->_update;
		}
	}

	BillboardPara::BillboardPara()
	{

	}

	BillboardPara::~BillboardPara()
	{

	}
}
