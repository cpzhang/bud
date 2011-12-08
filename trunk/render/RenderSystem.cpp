#include "RenderSystem.h"
#include "RenderEngineCreationParameters.h"
#include "Rect.h"
#include "external/directx/Include/d3dx9.h"
#include "external/directx/Include/DxErr.h"
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
		if (0 && _d3d9)
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

	bool RenderSystem::destroy()
	{
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


		D3DVIEWPORT9 pViewport;
		_device->GetViewport(&pViewport);
		return true;
	}

}