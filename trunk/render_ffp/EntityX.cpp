#include "stdafx.h"
#include "EntityX.h"
#include "IRenderSystem.h"
#include "Node.h"
#include "D3D9Mapping.h"
#include "Paras.h"
#include "TextureFactory.h"
#include "ITexture.h"
#include "Camera.h"
#include "IEffect.h"

//
namespace Euclid
{
	EntityX::EntityX()
		:	_mesh(0), _materials(0), _numMaterial(0), _xMaterialBuffer(0), _boundingSphere(NULL)
	{
		_type = "EntityX";
		_device = Paras::getInstancePtr()->_device;
	}

	EntityX::~EntityX()
	{
	}

	bool EntityX::create()
	{
		RenderableObject::create();

		return true;
	}

	void EntityX::destroy()
	{
		RenderableObject::destroy();

		//
		if (_materials)
		{
			delete[] _materials;
			_materials = NULL;
		}

		//
		for (size_t i = 0; i != _textures.size(); ++i)
		{
			if (_textures[i])
			{
				_textures[i]->release();
				_textures[i] = 0;
			}
		}
		_textures.clear();

		if (_mesh)
		{
			_mesh->Release();
			_mesh = 0;
		}

		//
// 		if (_texEffect)
// 		{
// 			_texEffect->destroy();
// 			delete _texEffect;
// 			_texEffect = NULL;
// 		}
	}

	void EntityX::preRender()
	{
		if (_currentNode)
		{
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	

			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			//
			//_renderSystem->setRenderState(D3DRS_LIGHTING, false);
			//
			//_renderSystem->setRenderState(D3DRS_AMBIENT, 0xffffffff);
			//
			_renderSystem->setRenderState(D3DRS_ZENABLE, true);
			//
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);
		}
	}

	void EntityX::postRender()
	{
		//
		_renderSystem->setVertexDeclaration(VD_NULL);
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		//
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(Euler::Matrix4::IDENTITY);
		_renderSystem->setWorldMatrix(&m);
	}

// 	void EntityX::render()
// 	{
// 		//
// 		if (!_mesh)
// 		{
// 			return;
// 		}
	// // #define MAX_LIGHTS 3
	// // 		D3DXVECTOR3 vLightDir[MAX_LIGHTS];
	// // 		D3DXCOLOR vLightDiffuse[MAX_LIGHTS];
	// // 
	// // 		// Render the light arrow so the user can visually see the light dir
	// // 		for( int i = 0; i < MAX_LIGHTS; i++ )
	// // 		{
	// // 			vLightDir[i] = *(D3DXVECTOR3*)(&Vec3(Euler::Basic::randomReal(-1, 1)));
	// // 			vLightDiffuse[i] = Euler::Basic::randomReal(0x0, 0xffffffff);
	// // 		}
	// // 
	// // 		_effect->setValue( "g_LightDir", vLightDir, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS );
	// // 		_effect->setValue( "g_LightDiffuse", vLightDiffuse, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS );
	// 
// 
// 		//
// 		// Update the effect's variables.  Instead of using strings, it would 
// 		// be more efficient to cache a handle to the parameter by calling ID3DXEffect::GetParameterByName
// // 		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
// // 		D3DXMATRIX v;
// // 		Paras::getInstancePtr()->_camera->getViewMatrix(v);
// // 		D3DXMATRIX p;
// // 		D3DXMatrixPerspectiveFovLH(&p, D3DX_PI * 0.45, 1.0f, 0.001f, 10000.f);
// // 		
// // 		_effect->setMatrix("g_mWorldViewProjection", &(m * v * p));
// // 		_effect->SetMatrix("g_mWorld", &m);
// // 		_effect->SetFloat("g_fTime", 0.0f/*Euler::Basic::randomReal(-3.14f, 3.14f)*/);
// // 
// // 		D3DXCOLOR vWhite = D3DXCOLOR( 1, 1, 1, 1 );
// // 		_effect->SetValue( "g_MaterialDiffuseColor", &vWhite, sizeof( D3DXCOLOR ) );
// // 		_effect->SetFloat( "g_fTime", 0.0f );
// // 		_effect->SetInt( "g_nNumLights", 3);
// // 
// // 		_effect->SetTechnique("RenderSceneWithTexture1Light");
// 
// 		//
// 		HRESULT hr;
// 		
// 		u32 effectNum = 0;
// 		// Apply the technique contained in the effect 
// 		_effect->begin(&effectNum);
// 
// 		for (u32 i = 0; i < effectNum; ++i)
// 		{
// 			_effect->beginPass(i);
// 
// 
// // 			for (unsigned int i = 0; i < _numMaterial; ++i)
// // 			{
// // 				// the mesh was loaded and is now ready to be rendered. 
// 				// It is divided into a subset for each material that was loaded for the mesh
// 				//
// // 				_device->SetMaterial(_materials + i);
// // 
// // 				//
// // 				_device->SetTexture(0, _textures[i]);
// 
// 				//
// 				hr = _mesh->DrawSubset(i);
// 				if (FAILED(hr))
// 				{
// 					return;
// 				}
// 			//}
// 
// 			_effect->endPass();
// 		}
// 
// 		_effect->end();
// 	}

	// D3DXMESH_SYSTEMMEM flag, which is equivalent to specifying both D3DXMESH_VB_SYSTEMMEM and D3DXMESH_IB_SYSTEMMEM.
	// Both of these flags put the index buffer and vertex buffer for the mesh in system memory.
	bool EntityX::loadMeshFromX( const std::string& file, unsigned long flag /*= D3DXMESH_SYSTEMMEM*/ )
	{
		std::string dataFile;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(dataFile);
		dataFile += "\\";
		dataFile += file;
		//
		HRESULT hr;
		hr = D3DXLoadMeshFromX(dataFile.c_str(), flag, _device, 0, &_xMaterialBuffer, 0, &_numMaterial, &_mesh);
		if (FAILED(hr))
		{
			std::ostringstream buf;
			buf<<"loadMeshFromX "<<file<<" Error Code : "<<GetLastError();
			Error(buf.str());
			return false;
		}

		if (_numMaterial == 0)
		{
			return false;
		}

		//
		if (!_xMaterialBuffer)
		{
			return false;
		}

		//
		_materials = new D3DMATERIAL9[_numMaterial];

		//
		//_textures = new LPDIRECT3DTEXTURE9[_numMaterial];

		//
		D3DXMATERIAL* mat = 0;
		mat = (D3DXMATERIAL*)_xMaterialBuffer->GetBufferPointer();

		//

		std::string tiger;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(tiger);
		//
		tiger += "\\image\\";
		for(unsigned int i = 0; i < _numMaterial; ++i)
		{
			//O
			_materials[i] = mat[i].MatD3D;
			_materials[i].Ambient = _materials[i].Diffuse;

			std::string fileName(tiger + mat[i].pTextureFilename);
			ITexture* t = TextureFactory::getInstancePtr()->createTextureFromFile(fileName);
			if (t == NULL)
			{
				std::ostringstream buf;
				buf<<"D3DXCreateTextureFromFile "<<fileName<<" Error Code : "<<GetLastError();
				Error(buf.str());
				return false;
			}
			_textures.push_back(t);
		}

		//
		_xMaterialBuffer->Release();

		//
		if (!(_mesh->GetFVF() & D3DFVF_NORMAL))
		{
			LPD3DXMESH temp;
			_mesh->CloneMeshFVF(_mesh->GetOptions(), _mesh->GetFVF() | D3DFVF_NORMAL, _device, &temp);
			D3DXComputeNormals(temp, NULL);
			_mesh->Release();
			_mesh = temp;
		}

		// Optimize the mesh for this graphics card's vertex cache 
		// so when rendering the mesh's triangle list the vertices will 
		// cache hit more often so it won't have to re-execute the vertex shader 
		// on those vertices so it will improve perf.     
		DWORD* rgdwAdjacency = NULL;
		rgdwAdjacency = new DWORD[_mesh->GetNumFaces() * 3];
		if( rgdwAdjacency == NULL )
			return E_OUTOFMEMORY;
		_mesh->GenerateAdjacency( 1e-6f, rgdwAdjacency );
		_mesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL );
		delete[] rgdwAdjacency;

		//
		return true;
	}

// 	bool EntityX::loadEffectFileFromFile( const std::string& file )
// 	{
// 		// Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
// 		// shader debugger. Debugging vertex shaders requires either REF or software vertex 
// 		// processing, and debugging pixel shaders requires REF.  The 
// 		// D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
// 		// shader debugger.  It enables source level debugging, prevents instruction 
// 		// reordering, prevents dead code elimination, and forces the compiler to compile 
// 		// against the next higher available software target, which ensures that the 
// 		// unoptimized shaders do not exceed the shader model limitations.  Setting these 
// 		// flags will cause slower rendering since the shaders will be unoptimized and 
// 		// forced into software.  See the DirectX documentation for more information about 
// 		// using the shader debugger.
// 		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
// 
// #if defined( DEBUG ) || defined( _DEBUG )
// 		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
// 		// Setting this flag improves the shader debugging experience, but still allows 
// 		// the shaders to be optimized and to run exactly the way they will run in 
// 		// the release configuration of this program.
// 		dwShaderFlags |= D3DXSHADER_DEBUG;
// #endif
// 
// #ifdef DEBUG_VS
// 		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
// #endif
// #ifdef DEBUG_PS
// 		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
// #endif
// 
// 		// Preshaders are parts of the shader that the effect system pulls out of the 
// 		// shader and runs on the host CPU. They should be used if you are GPU limited. 
// 		// The D3DXSHADER_NO_PRESHADER flag disables preshaders.
// 		//if( !g_bEnablePreshader )
// 		//	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
// 
// 		// Create an effect from an ASCII or binary effect description
// 		std::string data;
// 		Buddha::FileSystem::getInstancePtr()->getDataDirectory(data);
// 		data += file;
// 		if (FAILED(D3DXCreateEffectFromFile(_device, data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, NULL)))
// 		{
// 			std::ostringstream buf;
// 			buf<<"D3DXCreateTextureFromFile "<<file<<" Error Code : "<<GetLastError();
// 			Error(buf.str());
// 			return false;
// 		}
// 
// 		_texEffect = TextureFactory::getInstancePtr()->createTextureFromFile("x\\tiny\\tiny_skin.dds");
// 		if (_texEffect == NULL)
// 		{
// 			return false;
// 		}
// 
// 		// Set effect variables as needed
// 		D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
// 		D3DXCOLOR colorMtrlAmbient( 0.35f, 0.35f, 0.35f, 0 );
// 
// 		_effect->SetValue( "g_MaterialAmbientColor", &colorMtrlAmbient, sizeof( D3DXCOLOR ));
// 		_effect->SetValue( "g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof( D3DXCOLOR ));
// 		_effect->SetTexture( "g_MeshTexture", _texEffect->getTexture());
// 
// 		return true;
// 	}

	bool EntityX::getBoundingSphere(Sphere& s)
	{
		if (_boundingSphere == NULL)
		{
			Vec3* pData;
			if (FAILED(_mesh->LockVertexBuffer(0, (LPVOID*)&pData)))
			{
				std::ostringstream buf;
				buf<<"LockVertexBuffer Failed, Error Code : "<<GetLastError();
				Error(buf.str());
				return false;
			}
			Vec3 center;
			Real radius;
			if (FAILED(D3DXComputeBoundingSphere((D3DXVECTOR3*)pData, _mesh->GetNumVertices(), D3DXGetFVFVertexSize(_mesh->GetFVF()), (D3DXVECTOR3*)&center, &radius)))
			{
				std::ostringstream buf;
				buf<<"D3DXComputeBoundingSphere Failed, Error Code : "<<GetLastError();
				Error(buf.str());
				_mesh->UnlockVertexBuffer();
				return false;
			}

			_mesh->UnlockVertexBuffer();

			_boundingSphere = new Sphere(center, radius);
		}

		s = *_boundingSphere;

		return true;	
	}

	bool EntityX::getBoundingAABB( AABB& s )
	{
		if (_boundingAABB == NULL)
		{
			Vec3* pData;
			if (FAILED(_mesh->LockVertexBuffer(0, (LPVOID*)&pData)))
			{
				std::ostringstream buf;
				buf<<"LockVertexBuffer Failed, Error Code : "<<GetLastError();
				Error(buf.str());
				return false;
			}
			Vec3 min;
			Vec3 max;
			if (FAILED(D3DXComputeBoundingBox((D3DXVECTOR3*)pData, _mesh->GetNumVertices(), D3DXGetFVFVertexSize(_mesh->GetFVF()), (D3DXVECTOR3*)&min, (D3DXVECTOR3*)&max)))
			{
				std::ostringstream buf;
				buf<<"D3DXComputeBoundingBox Failed, Error Code : "<<GetLastError();
				Error(buf.str());
				_mesh->UnlockVertexBuffer();
				return false;
			}

			_mesh->UnlockVertexBuffer();

			_boundingAABB = new AABB(min, max);
		}

		s = *_boundingAABB;

		return true;	
	}

	void EntityX::renderNormal()
	{

	}

	void EntityX::renderWithEffect()
	{
		_renderSystem->setRenderState(D3DRS_ZENABLE, true);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//
		HRESULT hr;

		u32 effectNum = 0;
		// Apply the technique contained in the effect 
		_effect->begin(&effectNum);

		for (u32 i = 0; i < effectNum; ++i)
		{
			_effect->beginPass(i);

			for (unsigned int k = 0; k < _numMaterial; ++k)
			{
			 // the mesh was loaded and is now ready to be rendered. 
			 // It is divided into a subset for each material that was loaded for the mesh
// 				_device->SetMaterial(_materials + k);
//  				//
//  				_device->SetTexture(0, _textures[k]);

				_effect->setTexture("g_MeshTexture", _textures[k]);
				//
				hr = _mesh->DrawSubset(k);
				if (FAILED(hr))
				{
					return;
				}
			}

			_effect->endPass();
		}

		_effect->end();
	}

}
