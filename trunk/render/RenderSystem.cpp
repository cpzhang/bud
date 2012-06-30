#include "RenderSystem.h"
#include "RenderEngineCreationParameters.h"
#include "Rect.h"
#include "Buffer.h"
#include "ITexture.h"
#include "EffectManager.h"
namespace Euclid
{
	bool RenderSystem::create()
	{
		//
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (NULL  == _d3d9)
		{
			return false;
		}

		UINT adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
		HWND hFocusWindow = RenderEngineCreationParameters::getInstancePtr()->hFocusWindow;

		
//#define NV_PERF_HUD
#ifdef NV_PERF_HUD
		// Look for 'NVIDIA PerfHUD' adapter
		// If it is present, override default settings
		for(UINT adpt = 0;adpt < _d3d9->GetAdapterCount();adpt++)
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = _d3d9->GetAdapterIdentifier(adpt, 0, &Identifier);
			if (strstr(Identifier.Description,"PerfHUD") != 0)
			{
				adapter = adpt;
				deviceType = D3DDEVTYPE_REF;
				break;
			}
		}
#endif
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
		//D3DPRESENT_PARAMETERS pPresentationParameters;
		ZeroMemory(&_presentationParameters, sizeof(_presentationParameters));
		_presentationParameters.Windowed = true;
		_presentationParameters.hDeviceWindow = hFocusWindow;
		_presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		_presentationParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		_presentationParameters.hDeviceWindow = hFocusWindow;
		_presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		// The rate at which the display adapter refreshes the screen. 
		_presentationParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		{
			_presentationParameters.BackBufferCount = 1;
			// Use the current display mode.
			D3DDISPLAYMODE mode;

			if(FAILED(_d3d9->GetAdapterDisplayMode(adapter, &mode)))
			{
				return false;
			}

			_presentationParameters.BackBufferFormat = mode.Format;
			_presentationParameters.EnableAutoDepthStencil = true;
			_presentationParameters.AutoDepthStencilFormat	= D3DFMT_D16;
			//
			RECT rect;
			GetClientRect(hFocusWindow, &rect);
			// Width of the new swap chain's back buffers, in pixels
			_presentationParameters.BackBufferWidth	= rect.right - rect.left;

			// Height of the new swap chain's back buffers, in pixels. 
			_presentationParameters.BackBufferHeight = rect.bottom - rect.top;

			// Multisampling is supported only if the swap effect is D3DSWAPEFFECT_DISCARD.
			_presentationParameters.MultiSampleType			= D3DMULTISAMPLE_NONE;

			_presentationParameters.MultiSampleQuality = 0;

			_presentationParameters.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		}
		{
			HRESULT r = _d3d9->CreateDevice(adapter, deviceType, hFocusWindow, behaviorFlags, &_presentationParameters, &_device);
			if (D3D_OK != r)
			{
				throw EDX(r);
			}
		}
		//
		HRESULT r = _device->GetRenderTarget(0, &s_pRenderSurface);
		if (D3D_OK != r)
		{
			throw EDX(r);
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
		tstring _reason;
		{
			std::ostringstream ss;
			ss<<"dx error, string: "<<DXGetErrorString(D3DERR_OUTOFVIDEOMEMORY)<<";description: "<<DXGetErrorDescription(D3DERR_OUTOFVIDEOMEMORY);
			//ascii2unicode(ss.str().c_str(), _reason);
			_reason = ss.str();
		}
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
		//
		if (s_pRenderSurface)
		{
			ULONG reference_count = s_pRenderSurface->Release();
			if (reference_count > 0)
			{
				throw EReleaseLeak(reference_count);
			}
			s_pRenderSurface = NULL;
		}
		if (_device)
		{
			ULONG reference_count = _device->Release();
			if (reference_count > 0)
			{
				throw EReleaseLeak(reference_count);
			}
			_device = 0;
		}

		if (_d3d9)
		{
			ULONG reference_count = _d3d9->Release();
			if (reference_count > 0)
			{
				throw EReleaseLeak(reference_count);
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
			r = _device->TestCooperativeLevel();
			switch(r)
			{
				// Device lost -> Fullscreen window has lost focus (alt + tab, ctrl + alt + del, ...). Lets wait until we regain focus
			case D3DERR_DEVICELOST:
				{

				}break;
				// Device not reset -> We have regained focus and the device needs to be recreated
			case D3DERR_DEVICENOTRESET:
				{

				}break;
			default:
				{
					throw EDX(r);
				}break;
			}
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
			//	eVertexDeclarationType_PositionBoneWeight
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		20,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
				{0,		36,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
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
			//	eVertexDeclarationType_PositionTextureBoneWeight
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				{0,		20,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
				{0,		36,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
				D3DDECL_END()
			},
			//	eVertexDeclarationType_PositionTextureBoneWeightColor
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				{0,		20,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,	0},
				{0,		36,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,	0},
				{0,		52,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
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
			//	eVertexDeclarationType_PositionTTexture
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
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
			//	eVertexDeclarationType_PositionTextureNormalIndex
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				{ 0,	20, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
				{ 0,	32,	D3DDECLTYPE_FLOAT1,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1},
				D3DDECL_END()
			},
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

	bool RenderSystem::drawIndexedPrimitiveUP( ePrimitive PrimitiveType, u32 MinVertexIndex, u32 NumVertices, u32 PrimitiveCount, const void *pIndexData, eFormat IndexDataFormat, const void *pVertexStreamZeroData, u32 VertexStreamZeroStride )
	{
		HRESULT r = _device->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, (D3DFORMAT)IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}


	void RenderSystem::getViewPort( sViewPort* vp )
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

	bool RenderSystem::setRenderState( eRenderState State, u32 Value )
	{
		HRESULT r = _device->SetRenderState((D3DRENDERSTATETYPE)State, Value);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	bool RenderSystem::drawPrimitiveUP( ePrimitive PrimitiveType, u32 PrimitiveCount, const void *pVertexStreamZeroData, u32 VertexStreamZeroStride )
	{
		HRESULT r = _device->DrawPrimitiveUP((D3DPRIMITIVETYPE)PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::getRenderState( eRenderState State, u32 *pValue )
	{
		HRESULT r = _device->GetRenderState((D3DRENDERSTATETYPE)State, (DWORD*)pValue);
		if (D3D_OK != r)
		{
			return false;
		}

		return true;
	}

	bool RenderSystem::setViewport( const sViewPort *pViewport )
	{
		HRESULT r = _device->SetViewport(pViewport);
		if (D3D_OK != r)
		{
			return false;
		}
		s_pRenderSurface->Release();
		s_pRenderSurface = NULL;

		//
		_presentationParameters.BackBufferWidth = pViewport->Width;
		_presentationParameters.BackBufferHeight = pViewport->Height;
 		HRESULT hr = _device->Reset(&_presentationParameters);
 		if (D3D_OK != r)
 		{
 			throw EDX(hr);
 		}
		
		//
		r = _device->GetRenderTarget(0, &s_pRenderSurface);
		if (D3D_OK != r)
		{
			throw EDX(r);
		}
		return true;
	}

	bool RenderSystem::setRenderTarget( u32 RenderTargetIndex, ITexture* tex)
	{
		LPDIRECT3DSURFACE9 pRenderSurface = NULL;
		if (NULL == tex	)
		{
			pRenderSurface = s_pRenderSurface;
		}
		else
		{

			IDirect3DTexture9* t = tex->getTexture();
			t->GetSurfaceLevel(0, &pRenderSurface);
			//
			if (pRenderSurface)
			{
				pRenderSurface->Release();
			}
		}
	
		//
		HRESULT r = _device->SetRenderTarget(RenderTargetIndex, pRenderSurface);
		if (D3D_OK != r)
		{
			throw EDX(r);
		}
	}

	Vec3* RenderSystem::objectSpace2ScreenSpace( __inout Vec3 *pOut, __in const Vec3 *pV, __in const sViewPort *pViewport, __in const Mat4 *pProjection, __in const Mat4 *pView, __in const Mat4 *pWorld )
	{
		sViewPort vp;
		getViewPort(&vp);
		D3DXVec3Project((D3DXVECTOR3*)pOut, (D3DXVECTOR3*)pV, &vp, (D3DXMATRIX*)(&pProjection->transpose()), (D3DXMATRIX*)(&pView->transpose()), (D3DXMATRIX*)(&pWorld->transpose()));
		return pOut;
	}

	LPDIRECT3DSURFACE9 RenderSystem::s_pRenderSurface(NULL);
}