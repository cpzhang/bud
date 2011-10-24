#include "stdafx.h"
#include "SkyBox.h"
#include "BufferFactory.h"
#include "IRenderSystem.h"
#include "D3D9Mapping.h"
#include "Node.h"
#include "MD2Loader.h"
#include "ZIPManager.h"
#include "IFileRead.h"
#include "TextureFactory.h"
#include "ITexture.h"
//
namespace Euclid
{
	SkyBox::SkyBox()
	{
		_type = "SkyBox";
		for (int i = 0; i < eSkyBoxPlane_Max; ++i)
		{
			_tex[i] = NULL;
		}
	}

	SkyBox::~SkyBox()
	{
		destroy();
	}

	bool SkyBox::create()
	{
		if (!_initGeometry())
		{
			return false;
		}
		//
		return true;
	}

	void SkyBox::render()
	{
		for (unsigned char p = eSkyBoxPlane_X_Positive; p != eSkyBoxPlane_Max; ++p)
		{
			_renderSystem->drawQuad(cGeometry[p], _tex[p]);
		}
	}

	void SkyBox::preRender()
	{//
		if (_currentNode)
		{
			/*_renderSystem->setRenderState(D3DRS_STENCILENABLE, false);
			_renderSystem->setRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			_renderSystem->setRenderState(D3DRS_STENCILREF, 0x1);
			_renderSystem->setRenderState(D3DRS_STENCILMASK, 0xffffffff);
			_renderSystem->setRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
			_renderSystem->setRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			_renderSystem->setRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			_renderSystem->setRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);*/

			_renderSystem->setRenderState(D3DRS_ZWRITEENABLE, true);

			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//
			_renderSystem->setRenderState(D3DRS_LIGHTING, false);

			_renderSystem->setSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			_renderSystem->setSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			_renderSystem->setSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);

			_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			//_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			
			//
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);
		}
	}

	void SkyBox::postRender()
	{
		/*_renderSystem->setRenderState(D3DRS_STENCILENABLE, false);*/

		_renderSystem->setSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		_renderSystem->setSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

		_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		//
		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		//
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(Euler::Matrix4::IDENTITY);
		_renderSystem->setWorldMatrix(&m);
	}

	void SkyBox::destroy()
	{
		for (unsigned char p = eSkyBoxPlane_X_Positive; p != eSkyBoxPlane_Max; ++p)
		{
			if (_tex[p] != NULL)
			{
				_tex[p]->destroy();
			}
		}
	}

	bool SkyBox::_initGeometry()
	{
		IFileRead* f = NULL;

		for (unsigned char p = eSkyBoxPlane_X_Positive; p != eSkyBoxPlane_Max; ++p)
		{
			f = ZIPManager::getInstancePtr()->getFileReader(cTextureFileName[p]);

			if (f == NULL)
			{
				return false;
			}

			f->seek(0, false);

			unsigned char* buffer = new unsigned char[f->getSize()];
			
			f->read(buffer, f->getSize());

			_tex[p] = TextureFactory::getInstancePtr()->createTexFromMemory(buffer, f->getSize());

			delete[] buffer;

			if (_tex[p] == NULL)
			{
				return false;
			}
		}

		Record("Loaded Skybox successfully");
		
		return true;
	}

	const POSITION_TEXTURE SkyBox::cGeometry[eSkyBoxPlane_Max][4] = 
	{
		// posx
		{
			POSITION_TEXTURE( 0.5f,  0.5f,  0.5f, 0.0f, 0.0f),
			POSITION_TEXTURE( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE( 0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
			POSITION_TEXTURE( 0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
		},

		//	negx
		{
			POSITION_TEXTURE( -0.5f,  0.5f,  0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE( -0.5f,  0.5f, -0.5f, 0.0f, 0.0f),
			POSITION_TEXTURE( -0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
			POSITION_TEXTURE( -0.5f, -0.5f,  0.5f, 1.0f, 1.0f),
		},

		//	posy
		{
			POSITION_TEXTURE( -0.5f, 0.5f,  0.5f, 0.0f, 1.0f),
			POSITION_TEXTURE(  0.5f, 0.5f,  0.5f, 1.0f, 1.0f),
			POSITION_TEXTURE(  0.5f, 0.5f, -0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE( -0.5f, 0.5f, -0.5f, 0.0f, 0.0f),
		},

		//	negy
		{
			POSITION_TEXTURE( -0.5f, -0.5f,  0.5f, 0.0f, 0.0f),
			POSITION_TEXTURE(  0.5f, -0.5f,  0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE(  0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
			POSITION_TEXTURE( -0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
		},

		//	posz
		{
			POSITION_TEXTURE(-0.5f,  0.5f, 0.5f, 0.0f, 0.0f),
			POSITION_TEXTURE( 0.5f,  0.5f, 0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE( 0.5f, -0.5f, 0.5f, 1.0f, 1.0f),
			POSITION_TEXTURE(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f),
		},

		//	negz
		{
			POSITION_TEXTURE(-0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
			POSITION_TEXTURE( 0.5f,  0.5f, -0.5f, 0.0f, 0.0f),
			POSITION_TEXTURE( 0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
			POSITION_TEXTURE(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
		}
	};

	const char SkyBox::cTextureFileName[eSkyBoxPlane_Max][10] = 
	{
		"posx.jpg",

		"negx.jpg",

		"posy.jpg",

		"negy.jpg",

		"posz.jpg",

		"negz.jpg"
	};

}
 
