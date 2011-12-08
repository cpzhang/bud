#include "LightningStrip.h"
#include "VertexDeclaration.h"
#include "BufferFactory.h"
#include "IRenderSystem.h"
#include "D3D9Mapping.h"
#include "Node.h"
#include "MD2Loader.h"
#include "ZIPManager.h"
#include "IFileRead.h"
#include "TextureFactory.h"
//
namespace Euclid
{
	LightningStrip::LightningStrip()
		:	_tex(NULL)
	{
		_vb = NULL;
		_type = "LightningStrip";
		_bufferManager = BufferFactory::getInstancePtr();
		_begin = Vec3( 0.0f,	 0.0f,	0.0f);
		_end   = Vec3(  0.15f,	 0.0f,	0.0f);
		_cell_width = 0.01f;
		_cell_num = (_begin - _end).length() / _cell_width;
		_vertices_num = (int)(2.0f*_cell_num) + 2;
	}

	LightningStrip::~LightningStrip()
	{
		destroy();
	}

	bool LightningStrip::create()
	{
		IFileRead* f = NULL;

		f = ZIPManager::getInstancePtr()->getFileReader("strip.dds");
		//f = ZIPManager::getInstancePtr()->getFileReader("negx.jpg");

		if (f == NULL)
		{
			Error("Failed to get the File Reader");
			return false;
		}

		f->seek(0, false);

		unsigned char* buffer = new unsigned char[f->getSize()];

		f->read(buffer, f->getSize());

		_tex = TextureFactory::getInstancePtr()->createTexFromMemory(buffer, f->getSize());

		if (buffer)
		{
			delete[] buffer;
			buffer = 0;
		}

		if (_tex == NULL)
		{
			Error("Failed to create Texture!");
			return false;
		}
		
		_vb = BufferFactory::getInstancePtr()->createVertexBuffer(_vertices_num * sizeof(POSITION_COLOR_TEXTURE),
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
1__3__5__7
|\ |\ |\ |
| \| \| \|
0--2--4--6
*/
		// 初始化贴图坐标和颜色
		const Vec2 uv[4] = 
		{
			Vec2(0.f, 1.f),
			Vec2(0.f, 0.f),
			Vec2(1.f, 1.f),
			Vec2(1.f, 0.f)
		};

		for (int i = 0; i < _vertices_num; ++i)
		{
			v[i]._tex.x = uv[i%4].x;
			v[i]._tex.y = uv[i%4].y;

			v[i]._color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		_vb->unLock();

		//
		return true;
	}

	void LightningStrip::render()
	{
		if (_tex == NULL)
		{
			return;
		}
		_renderSystem->setTexture(0, _tex);


		_renderSystem->setStreamSource(0, _vb, 0, sizeof(POSITION_COLOR_TEXTURE));
		//
		_renderSystem->drawPrimitive(D3DPT_TRIANGLESTRIP, 0, _vertices_num - 2);
	}

	void LightningStrip::preRender()
	{
		//update();
		//
		if (_currentNode)
		{
			_renderSystem->setVertexDeclaration(VD_POSITION_COLOR_TEXTURE);

			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//
			_renderSystem->setRenderState(D3DRS_LIGHTING, false);
			//
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);

			_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
			_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

			/*_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);*/
		}
	}

	void LightningStrip::postRender()
	{
		_renderSystem->setTexture(0, 0);
		//
		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		//
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(Euler::Matrix4::IDENTITY);
		_renderSystem->setWorldMatrix(&m);

		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void LightningStrip::destroy()
	{
		if (_vb)
		{
			_vb->destroy();
		}
	}

	void LightningStrip::update(u32 current, u32 delta)
	{
		static unsigned long lastTime = timeGetTime();
		unsigned long currentTime = timeGetTime();
		if (currentTime - lastTime <= 10)
		{
			return;
		}
		lastTime = currentTime;

		POSITION_COLOR_TEXTURE* v = NULL;
		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
		if (v == NULL)
		{
			Error("Failed to Lock the Vertex Buffer");
			return;
		}

		Vec3 beg = _begin;
/*
1__3__5__7
|\ |\ |\ |
| \| \| \|
0--2--4--6
*/
		static const Real Height = 0.015f;
		static const Vec3 VHeight = Vec3(0.0f, Height, 0.0f);
		v[0]._pos = _begin;
		v[1]._pos = _begin;
		v[_vertices_num - 2]._pos = _end;
		v[_vertices_num - 1]._pos = _end;

		for (int i = 0; i < _cell_num; ++i)
		{
			v[2*(i+1)]._pos.x = Euler::Basic::randomReal(i*_cell_width, (i+1)*_cell_width);
			v[2*(i+1)]._pos.y = Euler::Basic::randomReal(-Height, 0.0f) - Height;

			v[2*(i+1) + 1]._pos.x = Euler::Basic::randomReal(i*_cell_width, (i+1)*_cell_width);
			v[2*(i+1) + 1]._pos.y = Euler::Basic::randomReal(0.0f, Height) + Height;
		}

		_vb->unLock();
	}
}
 
