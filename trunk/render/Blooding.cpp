#include "stdafx.h"
#include "Blooding.h" 
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
	Blooding::Blooding()
		:	_tex(NULL)
	{
		_vb = NULL;
		_type = "Blooding";
		_bufferManager = BufferFactory::getInstancePtr();
		_begin = Vec3( 0.0f,	 0.0f,	0.0f);
		_end   = Vec3(  0.6f,	 0.0f,	0.0f);
		_cell_width = 0.06f;
		_cell_num = (_begin - _end).length() / _cell_width;
		_vertices_num = 2*_cell_num + 4;
	}

	Blooding::Blooding( std::string texFile, Real speed )
		:	_tex(NULL), _texFile(texFile), _rotateSpeed(speed)
	{
		_vb = NULL;
		_type = "Blooding";
		_bufferManager = BufferFactory::getInstancePtr();
		_begin = Vec3(  -0.3f,	 0.0f,	0.0f);
		_end   = Vec3(  0.3f,	 0.0f,	0.0f);
		_cell_width = 0.015f;
		_cell_num = (_begin - _end).length() / _cell_width;
		_vertices_num = 2*_cell_num + 4;
		_j = 0;
		_lastJ = 0;
	}

	Blooding::Blooding( ManualPara* para )
	{
		_flowedU = 0.0f;
		_para = new ManualPara;
		memcpy(_para, para, sizeof(ManualPara));
		_indexU = 0;
		_tex = NULL;
		_updateUV = para->_updateUV;
		_updateVertex = para->_updateVertex;
		_texFile = para->_texFile;
		_rotateSpeed = para->_rotateSpeed;
		_height = para->_height;
		_heightEx = para->_heightEx;
		_vb = NULL;
		_type = "Blooding";
		_bufferManager = BufferFactory::getInstancePtr();
		_begin = Vec3(  -0.3f,	 0.0f,	0.0f);
		_end   = Vec3(  0.3f,	 0.0f,	0.0f);
		_cell_width = 0.006f;
		_cell_num = (_begin - _end).length() / _cell_width;
		_vertices_num = 2*_cell_num + 4;
		_j = 0;
		_lastJ = 0;
	}
	Blooding::~Blooding()
	{
		destroy();
	}

	bool Blooding::create()
	{
		/*IFileRead* f = NULL;

		f = ZIPManager::getInstancePtr()->getFileReader(_texFile);

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
		}*/
		std::string dataPath;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(dataPath);
		_texFile = dataPath + "/image/" + _texFile;
		_tex = TextureFactory::getInstancePtr()->createTextureFromFile(_texFile);
		if (_tex == NULL)
		{
			Error("Failed to create Texture!" + _texFile);
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

		_u = new Vec2[_vertices_num];
		for (int i = 0; i < _vertices_num; ++i)
		{
			if (i % 2 == 0)
			{
				v[i]._tex.x = ((float)i / (_vertices_num-1))*1.0f;
			}
			else
			{
				v[i]._tex.x = v[i-1]._tex.x; //((float)(i-1) / (_vertices_num-1))*1.0f;
			}
			
			v[i]._tex.y = uv[i%4].y;
			
			_u[i] = v[i]._tex;
			
			v[i]._color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		Vec3 beg = _begin;
/*
1__3__5__7
|\ |\ |\ |
| \| \| \|
0--2--4--6
*/
		Real Height = _height;
		Vec3 VHeight = Vec3(0.0f, Height, 0.0f);
		v[0]._pos = _begin - VHeight;
		v[1]._pos = _begin + VHeight;
		v[_vertices_num - 2]._pos = _end - VHeight;
		v[_vertices_num - 1]._pos = _end + VHeight;

		for (int i = 1; i <= _cell_num; ++i)
		{
			beg.x += _cell_width;
			
			v[2*i]._pos = beg - VHeight; 

			v[2*i + 1]._pos = beg + VHeight;
		}

		_vb->unLock();

		Record("Success to Create Texture!");
		//
		return true;
	}

	void Blooding::render()
	{
		if (_tex == NULL)
		{
			return;
		}
		_renderSystem->setTexture(0, _tex);

		if (_updateUV)
		{
			_renderSystem->setTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
			Mat4 mat = Mat4::IDENTITY;
			mat[0][2] = _flowedU;
			_renderSystem->setTextureMatrix(0, mat);
		}
		_renderSystem->setStreamSource(0, _vb, 0, sizeof(POSITION_COLOR_TEXTURE));
		//
		_renderSystem->drawPrimitive(D3DPT_TRIANGLESTRIP, 0, _vertices_num - 2);
	} 

	void Blooding::preRender()
	{
		//update();
		//
		if (_currentNode)
		{
			//_currentNode->pitch(_rotateSpeed, Euclid::Node::TS_LOCAL);

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
			_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			//_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			//_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		}
	}

	void Blooding::postRender()
	{
		_renderSystem->setTexture(0, 0);
		//
		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		//
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(Euler::Matrix4::IDENTITY);
		_renderSystem->setWorldMatrix(&m);

		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void Blooding::destroy()
	{
		if (_vb)
		{
			_vb->destroy();
		}

		if (_u)
		{
			delete[] _u;
			_u = 0;
		}
	}

	void Blooding::update(u32 current, u32 delta)
	{
		if (_updateUV)
		{
			_flowedU += 0.001 * delta * _para->_uvSpeed;
			/*if (!updateUV())
			{
				return;
			}*/
		}

		if (_updateVertex)
		{
			if (!updateVertex())
			{
				return;
			}
		}

		if (_para->_rotate)
		{
			if (!rotation_pitch())
			{
				return;
			}
		}
	}

	bool Blooding::updateUV()
	{
		static unsigned long lastTime = timeGetTime();
		unsigned long currentTime = timeGetTime();
		if (currentTime - lastTime <= 0)
		{
			//return true;
		}
		lastTime = currentTime;

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
| \| \| \|0
0--2--4--6
*/
		if (_indexU >= _vertices_num || _indexU < 0)
		{
			_indexU = 0;
		}
		for (int i = 0; i < _vertices_num; ++i)
		{	
			if (i % 2 == 0)
			{
				int j = i - _indexU;
				if (j >= 0)
				{
					v[i]._tex = _u[j];
				}
				else
				{
					v[i]._tex = _u[j + _vertices_num];
				}
			}
			else
			{
				v[i]._tex.x = v[i - 1]._tex.x;
			}
		}


		_vb->unLock();

		_indexU += _para->_uvSpeed;

		return true;
	}

	bool Blooding::updateVertex()
	{
		static unsigned long lastTime = timeGetTime();
		unsigned long currentTime = timeGetTime();
		if (currentTime - lastTime <= 0)
		{
			//return true;
		}
		lastTime = currentTime;

		POSITION_COLOR_TEXTURE* v = NULL;
		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
		if (v == NULL)
		{
			Error("Failed to Lock the Vertex Buffer");
			return false;
		}

		Vec3 beg = _begin;
/*
1__3__5__7
|\ |\ |\ |
| \| \| \|0
0--2--4--6
*/
		Real Height = _height;
		Vec3 VHeight = Vec3(0.0f, Height, 0.0f);
		v[0]._pos = _begin;
		v[1]._pos = _begin + VHeight;
		v[_vertices_num - 2]._pos = _end;
		v[_vertices_num - 1]._pos = _end + VHeight;

		_j += _para->_vertexSpeed;

		if (_j > _cell_num)
		{
			_j = 1;
		} 
		static const int num = 5;
		for (int i = 1; i <= _cell_num; ++i)
		{
			beg.x += _cell_width;
			
			v[2*i]._pos = beg - VHeight; 
			v[2*i + 1]._pos = beg + VHeight;

			if (i >= _j && i <= _j+num)
			{
				float t = 3.1415926*0.5*(i - _j - 0.5f*num)/(num*0.5); 
				float r = 0.5*Euler::Basic::Cos(t) + 0.0f;
				v[2*i]._pos -= Vec3(0.0f, _heightEx, 0.0f) * r;
				v[2*i + 1]._pos += Vec3(0.0f, _heightEx, 0.0f) * r;
			}
		}


		_vb->unLock();

		return true;
	}

	bool Blooding::rotation_pitch()
	{
		if (_currentNode)
		{
			_currentNode->pitch(_rotateSpeed, Euclid::Node::TS_LOCAL);
		}

		return true;
	}
}