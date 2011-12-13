#include "RenderSystem.h"
#include "RenderEngineCreationParameters.h"
#include "Rect.h"
#include "Buffer.h"
#include "ITexture.h"
namespace Euclid
{
	bool RenderSystem::create()
	{
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (NULL  == _d3d9)
		{
			return false;
		}

		UINT adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
		HWND hFocusWindow = RenderEngineCreationParameters::getInstancePtr()->hFocusWindow;
		
		//
		D3DCAPS9				_caps;
		_d3d9->GetDeviceCaps(adapter, deviceType, &_caps);
		DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if (_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else
		{
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//
		D3DPRESENT_PARAMETERS pPresentationParameters;
		ZeroMemory(&pPresentationParameters, sizeof(pPresentationParameters));
		pPresentationParameters.Windowed = true;
		pPresentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pPresentationParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		pPresentationParameters.hDeviceWindow = hFocusWindow;
		pPresentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		// The rate at which the display adapter refreshes the screen. 
		pPresentationParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		{
			pPresentationParameters.BackBufferCount = 1;
			// Use the current display mode.
			D3DDISPLAYMODE mode;

			if(FAILED(_d3d9->GetAdapterDisplayMode(adapter, &mode)))
			{
				return false;
			}

			pPresentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;//mode.Format;
			pPresentationParameters.EnableAutoDepthStencil = true;
			pPresentationParameters.AutoDepthStencilFormat	= D3DFMT_D24S8;
			//
			//
			RECT rect;
			GetClientRect(hFocusWindow, &rect);
			// Width of the new swap chain's back buffers, in pixels
			pPresentationParameters.BackBufferWidth	= rect.right - rect.left;

			// Height of the new swap chain's back buffers, in pixels. 
			pPresentationParameters.BackBufferHeight = rect.bottom - rect.top;

			// Multisampling is supported only if the swap effect is D3DSWAPEFFECT_DISCARD.
			pPresentationParameters.MultiSampleType			= D3DMULTISAMPLE_NONE;

			pPresentationParameters.MultiSampleQuality = 0;

			pPresentationParameters.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		}
		{
			HRESULT r = _d3d9->CreateDevice(adapter, deviceType, hFocusWindow, behaviorFlags, &pPresentationParameters, &_device);
			if (D3D_OK != r)
			{
				char buf[2048];
				sprintf(buf, "Error: %s error description: %s\n",DXGetErrorString(r),DXGetErrorDescription(r));
				OutputDebugString(buf);
				return false;
			}
		}

		// 这里可以清除IDirect3D9，此时PIX无法使用
/*
		if (_d3d9)
		{
			ULONG reference_count = _d3d9->Release();
			if (reference_count > 0)
			{
				return false;
			}
			_d3d9 = 0;
		}
*/
		// init vertex declaration
		initVertexDeclarations();
		return true;
	}

	bool RenderSystem::destroy()
	{
		//
		for (unsigned char i = eVertexDeclarationType_Null; i != eVertexDeclarationType_Size; ++i)
		{
			if (_vertexDeclarations[i])
			{
				_vertexDeclarations[i]->Release();
				_vertexDeclarations[i] = 0;
			}
		}

		if (_device)
		{
			ULONG reference_count = _device->Release();
			if (reference_count > 0)
			{
				return false;
			}
			_device = 0;
		}

		if (_d3d9)
		{
			ULONG reference_count = _d3d9->Release();
			if (reference_count > 0)
			{
				return false;
			}
			_d3d9 = 0;
		}

		return true;
	}

	RenderSystem::RenderSystem()
		:_d3d9(0), _device(0)
	{

	}

	RenderSystem::~RenderSystem()
	{

	}

	bool RenderSystem::clear( u32 Count, const Rect* pRects, u32 Flags, Color Color, float Z, u32 Stencil )
	{
		//
		D3DRECT rt;
		D3DRECT* pRT = NULL;
		ZeroMemory(&rt, sizeof(rt));
		if (pRects)
		{
			pRects->toD3DRECT(rt);
			pRT = &rt;
		}
		HRESULT r = _device->Clear(Count, pRT, Flags, Color.getARGB(), Z, Stencil);
		if (D3D_OK != r)
		{
			return false;
		}
		return true;
	}

	bool RenderSystem::beginScene()
	{
		HRESULT r = _device->BeginScene();
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::endScene()
	{
		HRESULT r = _device->EndScene();
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::present( const Rect *pSourceRect, const Rect *pDestRect, HWND hDestWindowOverride)
	{
		RECT *pSource = NULL;
		RECT rtSource;
		ZeroMemory(&rtSource, sizeof(rtSource));
		if (pSourceRect)
		{
			pSourceRect->toRECT(rtSource);
			pSource = &rtSource;
		}
		RECT* pDest = NULL;
		RECT rtDest;
		ZeroMemory(&rtDest, sizeof(rtDest));
		if (pDestRect)
		{
			pDestRect->toRECT(rtDest);
			pDest = &rtDest;
		}
		HRESULT r = _device->Present(pSource, pDest, NULL, NULL);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	void RenderSystem::initVertexDeclarations()
	{
		//
		static const u32 scMaxElementsNumber = 8;
		D3DVERTEXELEMENT9 ve[eVertexDeclarationType_Size][scMaxElementsNumber] = 
		{
			//	eVertexDeclarationType_Null
			{
				D3DDECL_END()
			},

			//	eVertexDeclarationType_Position
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				D3DDECL_END()
			},

			//	eVertexDeclarationType_PositionColor
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				D3DDECL_END()
			},

			//	eVertexDeclarationType_PositionTexture
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	eVertexDeclarationType_PositionTextureNormal
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
				D3DDECL_END()
			},

			//	eVertexDeclarationType_PositionColorTexture
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				{ 0,	16, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},
			// eVertexDeclarationType_Matrix
			{
				{ 1,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
				{ 1,	16,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2},
				{ 1,	32,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3},
				{ 1,	48,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4},
				D3DDECL_END()
			},
			//	eVertexDeclarationType_PositionTextureNormalMatrix
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
				{ 1,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
				{ 1,	16,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2},
				{ 1,	32,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3},
				{ 1,	48,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4},
				D3DDECL_END()
			},

/*
			//	VD_TPOSITION_COLOR
			{
				{0,		0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
				{0,		16,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				D3DDECL_END()
			},

			//	VD_POSITION_NORMAL
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
				D3DDECL_END()
			},

			//	VD_TPOSITION_TEXTURE
			{
				{0,		0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
				{0,		16,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	VD_TPOSITION_COLOR_TEXTURE
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
				{ 0,	16, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				{ 0,	20, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},
*/
		};

		//
		for (unsigned char i = eVertexDeclarationType_Null; i != eVertexDeclarationType_Size; ++i)
		{
			_device->CreateVertexDeclaration(ve[i], _vertexDeclarations + i);
		}
	}

	bool RenderSystem::setVertexDeclaration( eVertexDeclarationType e )
	{
		if (e >= eVertexDeclarationType_Null || e < eVertexDeclarationType_Size)
		{
			_device->SetVertexDeclaration(_vertexDeclarations[e]);
			return true;
		}
		return false;
	}

	IDirect3DDevice9* RenderSystem::getDevice()
	{
		return _device;
	}

	bool RenderSystem::setStreamSource( u32 StreamNumber, IBuffer *pStreamData, u32 OffsetInBytes, u32 Stride )
	{
		HRESULT r = _device->SetStreamSource(StreamNumber, pStreamData->getVertexBuffer(), OffsetInBytes, Stride);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::drawPrimitive( ePrimitive PrimitiveType, u32 StartVertex, u32 PrimitiveCount )
	{
		HRESULT r = _device->DrawPrimitive((D3DPRIMITIVETYPE)PrimitiveType, StartVertex, PrimitiveCount);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::drawIndexedPrimitive( ePrimitive Type, s32 BaseVertexIndex, u32 MinIndex, u32 NumVertices, u32 StartIndex, u32 PrimitiveCount )
	{

		HRESULT r = _device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}


	void RenderSystem::getViewPort( D3DVIEWPORT9* vp )
	{
		if (_device)
		{
			_device->GetViewport(vp);
		}
	}

	bool RenderSystem::setTexture( u32 Sampler, ITexture* pTexture )
	{
		if (pTexture)
		{
			HRESULT r = _device->SetTexture(Sampler, pTexture->getTexture());
			if (D3D_OK == r)
			{
				return true;
			}
		}

		return false;
	}

	bool RenderSystem::setStreamSourceFreq( u32 StreamNumber, u32 FrequencyParameter )
	{
		HRESULT r = _device->SetStreamSourceFreq(StreamNumber, FrequencyParameter);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	bool RenderSystem::setIndices( IBuffer *pIndexData )
	{
		if (NULL != pIndexData)
		{
			HRESULT r = _device->SetIndices(pIndexData->getIndexBuffer());
			if (D3D_OK == r)
			{
				return true;
			}
		}

		return false;
	}
}