#include "ManualObject.h"
#include "VertexDeclaration.h"
#include "BufferFactory.h"
#include "IRenderSystem.h"
#include "D3D9Mapping.h"
#include "Node.h"
#include "MD2Loader.h"
#include "ZIPManager.h"
#include "IFileRead.h"
#include "TextureFactory.h"
#include "Texture.h"
#include "Paras.h"
#include "IShader.h"
#include "IEffect.h"
#include "CameraEx.h"
#include "CameraControllerThirdPerson.h"
//
namespace Euclid
{
	ManualObject::ManualObject(const std::string& name)
		:RenderableObject(name), _unitSize(0), _pt(ePrimitiveType_TriangleList), _material(NULL), _light(NULL), _indices16(NULL), _indicesCount16(0), _indices32(NULL), _indicesCount32(0),
		_ib(NULL)
	{
		_vb = NULL;
		_type = "ManualObject";
		_bufferManager = BufferFactory::getInstancePtr();
		_begin = Vec3(  0.0f,	 0.0f,	0.0f);
		_end   = Vec3(  0.15f,	 0.0f,	0.0f);
		_cell_width = 0.01f;
		_cell_num = (_begin - _end).length() / _cell_width;
		_vertices_num = 2*_cell_num + 2;
		_wireFrame = false;
		_isSelected = false;

		_renderSystem = Paras::getInstancePtr()->_renderSystem;
		_primitiveCount = 0;
	}

	ManualObject::~ManualObject()
	{
		destroy();
	}

	bool ManualObject::create()
	{
// 		IFileRead* f = NULL;
// 
// 		f = ZIPManager::getInstancePtr()->getFileReader("strip.dds");
// 		//f = ZIPManager::getInstancePtr()->getFileReader("negx.jpg");
// 
// 		if (f == NULL)
// 		{
// 			Error("Failed to get the File Reader");
// 			return false;
// 		}
// 
// 		f->seek(0, false);
// 
// 		unsigned char* buffer = new unsigned char[f->getSize()];
// 
// 		f->read(buffer, f->getSize());
// 
// 		std::string path("e:\\20100423Refracture\\bin\\data\\image\\strip.dds");
// 		_tex = TextureFactory::getInstancePtr()->createTextureFromFile(path);
// 		//_tex = TextureFactory::getInstancePtr()->createTexFromMemory(buffer, f->getSize());
// 
// 		delete[] buffer;
// 
// 		if (_tex == NULL)
// 		{
// 			Error("Failed to create Texture!");
// 			return false;
// 		}
		
 		RenderableObject::create();

		_vb = BufferFactory::getInstancePtr()->createVertexBuffer(_vertices[_vdt].size() * _unitSize,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_DEFAULT); 

		if (_vb == NULL)
		{
			Error("Failed to create Vertex Buffer");
			return false;
		}
		u8* v = NULL;
		v = (u8*)_vb->lock(0, 0, D3DLOCK_DISCARD);

		// 		POSITION_COLOR_TEXTURE* v = NULL;
		// 		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
		if (v == NULL)
		{
			Error("Failed to Lock the Vertex Buffer");
			return false;
		}

		for (size_t i = 0; i != _vertices[_vdt].size(); ++i)
		{
			memcpy(v, (void*)_vertices[_vdt][i], _unitSize);
			v += _unitSize;
		}

		_vb->unLock();
		/*
1__3__5__7
|\ |\ |\ |
| \| \| \|
0--2--4--6
*/
	// 初始化贴图坐标和颜色
// 	const Euler::Vector2 uv[4] = 
// 	{
// 		Euler::Vector2(0.f, 1.f),
// 		Euler::Vector2(0.f, 0.f),
// 		Euler::Vector2(1.f, 1.f),
// 		Euler::Vector2(1.f, 0.f)
// 	};
// 
// 		for (int i = 0; i < _vertices_num; ++i)
// 		{
// 			v[i]._tex.x = uv[i%4].x;
// 			v[i]._tex.y = uv[i%4].y;
// 
// 			v[i]._color = D3DCOLOR_RGBA(255, 255, 255, 255);
// 		}

		

		if (_indices16 != NULL && _indicesCount16 > 0)
		{
			_ib = BufferFactory::getInstancePtr()->createIndexBuffer(2 * _indicesCount16, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT);
			if (_ib == NULL)
			{
				Error("Failed to create Index Buffer");
				return false;
			}
			u8* v = NULL;
			v = (u8*)_ib->lock(0, 0, D3DLOCK_DISCARD);
			if (v == NULL)
			{
				Error("Failed to Lock the Index Buffer");
				return false;
			}
			memcpy(v, (void*)_indices16, _indicesCount16 * 2);
			_ib->unLock();
		}
		else if (_indices32 != NULL && _indicesCount32 > 0)
		{
		}
		//
		return true;
	}

	void ManualObject::renderNormal()
	{
		//
		//_renderSystem->setStreamSource(0, _vb, 0, _unitSize);

		//
		RenderSpaceComponent* rsc = _space.getComponent("index buffer");
		RenderSpaceComponent_IndexBuffer* rscib = (RenderSpaceComponent_IndexBuffer*)rsc;
		IndexBuffer* ib = rscib->_ib;
		if (ib)
		{
			RenderSpaceComponent* rsc = _space.getComponent("vertex buffer");
			RenderSpaceComponent_VertexBuffer* rscvb = (RenderSpaceComponent_VertexBuffer*)rsc;
			//_renderSystem->setIndices(ib);
			_renderSystem->drawIndexedPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, 0, rscvb->_vertexCount, 0, _primitiveCount);
		}
		else
		{
			//
			//_renderSystem->drawPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, _getPrimitiveCount());
			_renderSystem->drawPrimitive(D3D9Mapping::convert2D3DPrimitiveType(_pt), 0, _primitiveCount);
		}
	}

	void ManualObject::preRender()
	{
		//update();
		//
		if (_currentNode)
		{
			_renderSystem->setVertexDeclaration(_vdt);
			
			if (_vertexShader && _currentNode)
			{
				_renderSystem->setVertexShader(_vertexShader);
			}
			else
			{
				if (_material != NULL && _light != NULL)
				{
					_renderSystem->setMaterial(_material);

					_renderSystem->setLight(0, _light);

					//
					_renderSystem->setRenderState(D3DRS_LIGHTING, true);
					//
					_renderSystem->setRenderState( D3DRS_AMBIENT, 0x00202020);
				}
				else
				{
					_renderSystem->setRenderState(D3DRS_LIGHTING, false);
				}
				if (_wireFrame)
				{
					_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				}
				else
				{
					_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				}
				
				_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				//
 				_currentNode->_updateFromParent();
				Mat4 worldMat = _currentNode->_getFullTransform();
				_renderSystem->setWorldMatrix(worldMat);

// 				Mat4 viewMat = Paras::getInstancePtr()->_cameraEx->getViewMatrix();
// 				Mat4 controllerMat = Mat4::IDENTITY;
// 				if (Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson())
// 				{
// 					controllerMat = Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson()->getMatrix();
// 				}
// 				
// 				Mat4 projectMat = _orthoProjection ? Paras::getInstancePtr()->_cameraEx->getOrthoProjectionMatrix():Paras::getInstancePtr()->_cameraEx->getProjectionMatrix();
// 				Mat4 finalMatrix = projectMat * viewMat * controllerMat * worldMat;
// 				
// 				_renderSystem->setViewMatrix(viewMat * controllerMat);
// 				_renderSystem->setProjectionMatrix(projectMat);

				_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
				_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				if (!_texs.empty())
				{
					for(size_t i = 0; i != _texs.size(); ++i)
					{
						_renderSystem->setTexture(i, _texs[i]);
// 						_renderSystem->setTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
// 						_renderSystem->setTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
// 						_renderSystem->setTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
// 
 						_renderSystem->setTextureStageState(i, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
 						_renderSystem->setTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 						_renderSystem->setTextureStageState(i, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	
					}
				}
				else
				{
					_renderSystem->setTexture(0, NULL);
				}
			}
		}
	}

	void ManualObject::postRender()
	{
		//
		if (_vertexShader && _currentNode)
		{
			_renderSystem->setVertexShader(NULL);
		}
// 		else
// 		{
// 			//
// 			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(Euler::Matrix4::IDENTITY);
// 			_renderSystem->setWorldMatrix(&m);
// 
// 			_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
// 
// 			for(size_t i = 0; i != _texs.size(); ++i)
// 			{
// 				_renderSystem->setTexture(i, NULL);
// 				_renderSystem->setTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
// 				_renderSystem->setTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
// 			}
// 
// 			//
// 			_renderSystem->setRenderState(D3DRS_LIGHTING, false);
// 		}
// 		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
// 		_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	void ManualObject::destroy()
	{
		RenderableObject::destroy();

		if (_ib)
		{
			_ib->destroy();
			delete _ib;
			_ib = NULL;
		}
		if (_vb)
		{
			_vb->destroy();
			delete _vb;
			_vb = NULL;
		}

		if (!_texs.empty())
		{
			for(size_t i = 0; i != _texs.size(); ++i)
			{
				if (_texs[i] != NULL)
				{
					_texs[i]->release();
				}
			}
			_texs.clear();
		}

		//
		for (int t = VD_NULL; t != VD_COUNT; ++t)
		{
			for (size_t i = 0; i != _vertices[t].size(); ++i)
			{
				switch(_vdt)
				{
				case VD_POSITION_COLOR:
					{
						POSITION_COLOR* p = (POSITION_COLOR*)_vertices[t][i];
						if (p)
						{
							delete p;
							p = NULL;
						}
					}break;
				case VD_POSITION_NORMAL:
					{
						POSITION_NORMAL* p = (POSITION_NORMAL*)_vertices[t][i];
						if (p)
						{
							delete p;
							p = NULL;
						}
					}break;
				case VD_POSITION_COLOR_TEXTURE:
					{
						POSITION_COLOR_TEXTURE* p = (POSITION_COLOR_TEXTURE*)_vertices[t][i];
						if (p)
						{
							delete p;
							p = NULL;
						}
					}break;
				}
			}
			_vertices[t].clear();
		}
	}

	void ManualObject::update(u32 current, u32 delta)
	{
		if(1){
			_currentNode->_updateFromParent();
			Mat4 worldMat = _currentNode->_getFullTransform();
			Mat4 viewMat = Paras::getInstancePtr()->_cameraEx->getViewMatrix();
			Mat4 controllerMat = Mat4::IDENTITY;
			if (Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson())
			{
				controllerMat = Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson()->getMatrix();
			}

			Mat4 projectMat = Paras::getInstancePtr()->_cameraEx->getProjectionMatrix();
			_space.getComponent("world transformation")->setVaule(worldMat);
			_space.getComponent("view transformation")->setVaule(viewMat * controllerMat);
			_space.getComponent("project transformation")->setVaule(projectMat);
		}
// 		if (_vertexShader && _currentNode)
// 		{
// 			_vertexShader->setMatrix("mWorldViewProj", _currentNode->_getFullTransform());
// 			_vertexShader->setFloat("fTime", (float)current * 0.0001f);
// 	//		_renderSystem->setVertexShader(_vertexShader);
// 		}
// 		static unsigned long lastTime = timeGetTime();
// 		unsigned long currentTime = timeGetTime();
// 		if (currentTime - lastTime <= 1000)
// 		{
// 			return;
// 		}
// 		lastTime = currentTime;
// 
// 		POSITION_COLOR_TEXTURE* v = NULL;
// 		v = (POSITION_COLOR_TEXTURE*)_vb->lock(0, 0, 0);
// 		if (v == NULL)
// 		{
// 			Error("Failed to Lock the Vertex Buffer");
// 			return;
// 		}
// 
// 		Vec3 beg = _begin;
// /*
// 1__3__5__7
// |\ |\ |\ |
// | \| \| \|
// 0--2--4--6
// */
// 		static const Real Height = 0.015f;
// 		static const Vec3 VHeight = Vec3(0.0f, Height, 0.0f);
// 		v[0]._pos = _begin;
// 		v[1]._pos = _begin;
// 		v[_vertices_num - 2]._pos = _end;
// 		v[_vertices_num - 1]._pos = _end;
// 
// 		for (int i = 0; i < _cell_num; ++i)
// 		{
// 			v[2*(i+1)]._pos.x = Euler::Basic::randomReal(i*_cell_width, (i+1)*_cell_width);
// 			v[2*(i+1)]._pos.y = Euler::Basic::randomReal(-Height, 0.0f) - Height;
// 
// 			v[2*(i+1) + 1]._pos.x = Euler::Basic::randomReal(i*_cell_width, (i+1)*_cell_width);
// 			v[2*(i+1) + 1]._pos.y = Euler::Basic::randomReal(0.0f, Height) + Height;
// 		}
// 
// 		_vb->unLock();
	}

	bool ManualObject::setVertexDeclarationType( eVertexDeclarationType t )
	{
		if (t <= VD_NULL || t >= VD_COUNT)
		{
			return false;
		}

		_vdt = t;

		return true;
	}


	void ManualObject::addVertex( const Vec3& p )
	{
		sPOSITION* v = new sPOSITION(p);
		_vertices[_vdt].push_back((u32)v);
	}

	void ManualObject::addVertex( const Vec3& p, RGBA c )
	{
		POSITION_COLOR* v = new POSITION_COLOR(p, c);
		_vertices[_vdt].push_back((u32)v);
	}

	void ManualObject::addVertex( const Vec3& p, const Vec3& n )
	{
		POSITION_NORMAL* v = new POSITION_NORMAL(p, n);
		_vertices[_vdt].push_back((u32)v);
	}

	void ManualObject::addVertex( const Vec3& p, RGBA c, const Vec2& t )
	{
		POSITION_COLOR_TEXTURE* v = new POSITION_COLOR_TEXTURE(p, c, t);
		_vertices[_vdt].push_back((u32)v);
	}

	void ManualObject::addVertex( const Vec3& p, const Vec2& t )
	{
		POSITION_TEXTURE* v = new POSITION_TEXTURE(p, t);
		_vertices[_vdt].push_back((u32)v);
	}

	bool ManualObject::setUnitSize( size_t i )
	{
		_unitSize = i;

		return true;
	}

	bool ManualObject::setPrimitiveType( ePrimitiveType t )
	{
		_pt = t;

		return true;
	}

	size_t ManualObject::_getPrimitiveCount()
	{
		size_t count = 0;

		switch(_pt)
		{
		case ePrimitiveType_PointList:
			{
				count = _vertices[_vdt].size();
			}break;

		case ePrimitiveType_LineList:
			{
				count = (size_t)(_vertices[_vdt].size() * 0.5f);
			}break;

		case ePrimitiveType_LineStrip:
			{
				count = (size_t)(_vertices[_vdt].size() - 1);
			}break;

		case ePrimitiveType_TriangleList:
			{
				if (_ib)
				{
					if (_indicesCount16 > 0)
					{
						count = _indicesCount16 / 3;
					}
					else if (_indicesCount32 > 0)
					{
						count = _indicesCount32 / 3;
					}
				}
				else
				{
					count = _vertices[_vdt].size() / 3;
				}
			}break;

		case ePrimitiveType_TriangleStrip:
			{
				count = _vertices[_vdt].size() - 2;
			}break;
		}

		return count;
	}

	bool ManualObject::setMaterial( Material* m )
	{
		_material = m;

		return true;
	}

	bool ManualObject::setLight( Light* l )
	{
		_light = l;

		return true;
	}

	void ManualObject::setTexture(u16 index,  ITexture* t )
	{
		_texs.push_back(t);
	}

	void ManualObject::setIndices( u16* d, u32 n )
	{
		_indices16 = d;
		_indicesCount16 = n;
	}

	void ManualObject::renderWithEffect()
	{
		{
			_currentNode->_updateFromParent();
			Mat4 worldMat = _currentNode->_getFullTransform();
			Mat4 viewMat = Paras::getInstancePtr()->_cameraEx->getViewMatrix();
			Mat4 controllerMat = Mat4::IDENTITY;
			if (Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson())
			{
				controllerMat = Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson()->getMatrix();
			}

			Mat4 projectMat = Paras::getInstancePtr()->_cameraEx->getProjectionMatrix();
			Mat4 finalMatrix = projectMat * viewMat * controllerMat * worldMat;

			_effect->setMatrix("g_mWorldViewProjection", &finalMatrix);
		}
		bool isSelected = _effect->getBool("g_isSelected");
		{
			_effect->setBool("g_isSelected", _isSelected);
		}
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

		_effect->setBool("g_isSelected", isSelected);
		_effect->end();
	}

	void ManualObject::setWireFrame( bool wf )
	{
		_wireFrame = wf;
	}

	ITexture* ManualObject::getTexture( u16 index )
	{
		if (index >= _texs.size())
		{
			return NULL;
		}
		return _texs[index];
	}

	void ManualObject::setSelected( bool b )
	{
		_isSelected = b;
	}

	bool ManualObject::setPrimitiveCount( size_t ptCount )
	{
		_primitiveCount = ptCount;
		return true;
	}

}
