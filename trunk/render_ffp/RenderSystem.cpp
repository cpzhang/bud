#include "stdafx.h"
#include "RenderSystem.h" 
#include "Paras.h"
#include "Camera.h"
#include "ITexture.h"
#include "Light.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "D3D9Mapping.h"
#include "IShader.h"
#include "Camera.h"
#include "CameraControllerThirdPerson.h"
//
namespace Euclid
{
	RenderSystem::RenderSystem()
		: _d3d9(0), _device(0), _3DMode(false)/*, _camera(NULL)*/
		, _clearColor(Color::Black)
	{
		 memset(_vertexDeclaration, 0, sizeof(_vertexDeclaration));
	}

	RenderSystem::~RenderSystem()
	{
		//destroy();
	}

	bool RenderSystem::create()
	{
		_hwnd = Paras::getInstancePtr()->_hwnd;

		//
		_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (NULL == _d3d9)
		{
			return false;
		}

		//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		unsigned int	adapter			= D3DADAPTER_DEFAULT;

		/*
		HAL (Hardware Abstraction Layer) devices and REF (REFerence rasterizer) devices are the two main types of D3D device;
		the first is based around the hardware support, and is very fast but might not support everything; 
		while the second uses no hardware acceleration, so is very slow, but is guaranteed to support the entire set of D3D features, 
		in the correct way. In general you'll only ever need to use HAL devices, but if you're using some advanced feature that your 
		graphics card does not support then you might need to fall back to REF.
		*/
		D3DDEVTYPE		deviceType		= D3DDEVTYPE_HAL;

		char tmp[512];
		//
		D3DADAPTER_IDENTIFIER9 ai;
		if (FAILED(_d3d9->GetAdapterIdentifier(adapter,0,&ai)))
		{
			return false;
		}

		unsigned short product = HIWORD(ai.DriverVersion.HighPart);
		unsigned short version = LOWORD(ai.DriverVersion.HighPart);
		unsigned short subVersion = HIWORD(ai.DriverVersion.LowPart);
		unsigned short build = LOWORD(ai.DriverVersion.LowPart);

		std::ostringstream ss;
		ss
		<<"Description: "<<ai.Description<<"</br>"
		<<"Driver: "<<ai.Driver<<"</br>"
		<<"Product: "<<product<<"</br>"
		<<"Version: "<<version<<"</br>"
		<<"SubVersion: "<<subVersion<<"</br>"
		<<"Build: "<<build;
		Record(ss);

#ifdef PerfHUD	
		// Look for 'NVIDIA PerfHUD' adapter
		// If it is present, override default settings
		for (UINT Adapter=0; Adapter< _d3d9->GetAdapterCount(); Adapter++) 
		{
			D3DADAPTER_IDENTIFIER9  Identifier;
			HRESULT  Res;
			Res = _d3d9->GetAdapterIdentifier(Adapter,0,&Identifier);
			if (strstr(Identifier.Description,"PerfHUD") != 0)
			{
				adapter = Adapter;
				deviceType = D3DDEVTYPE_REF;
				Record("NVIDIA PerfHUD");
				break;
			}
		}
#endif

		//
		_d3d9->GetDeviceCaps(adapter, deviceType, &_caps);
		DWORD	vp;// = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if (_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else
		{
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//
		if (_caps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST)
		{
		}
		else
		{

		}
		//
		int shaderCap = 0;
		if (queryShaderCaps(1, 0))
		{
			shaderCap = 1;
			if (queryShaderCaps(2, 0))
			{
				shaderCap = 2;
				if (queryShaderCaps(3, 0))
				{
					shaderCap = 3;
				}
			}
		}
		{
			std::ostringstream ss;
			ss<<"shader version: "<<shaderCap;
			Record(ss);
		}
		
		//vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX;

		// Debugging vertex shaders requires either REF or software vertex processing 
		// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
		if( deviceType != D3DDEVTYPE_REF )
		{
			vp &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			vp &= ~D3DCREATE_PUREDEVICE;
			vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
#endif
#ifdef DEBUG_PS
		deviceType = D3DDEVTYPE_REF;
#endif

		//
		_initPresentParas();

		//
		HRESULT hr;
		//
		if(FAILED(hr = _d3d9->CheckDeviceType(adapter, deviceType, _presentParas.BackBufferFormat, _presentParas.BackBufferFormat, FALSE)))
		{
			_presentParas.BackBufferFormat = /*D3DFMT_X8R8G8B8;//*/D3DFMT_R5G6B5;

			if(FAILED(hr = _d3d9->CheckDeviceType(adapter, deviceType, _presentParas.BackBufferFormat, _presentParas.BackBufferFormat, FALSE)))
			{
				// Use the current display mode.
				D3DDISPLAYMODE mode;

				if(FAILED(hr = _d3d9->GetAdapterDisplayMode(adapter, &mode)))
				{
					Error((hr));
					return false;
				}

				_presentParas.BackBufferFormat = mode.Format;

				if(FAILED(hr = _d3d9->CheckDeviceType(adapter, deviceType, _presentParas.BackBufferFormat, _presentParas.BackBufferFormat, FALSE)))
				{
					Error((hr));
					return false;
				}
			}
		}

		//	try creating a Hardware T&L device first
		hr = _d3d9->CreateDevice(adapter, deviceType, _hwnd, vp, &_presentParas, &_device);
		if (FAILED(hr))
		{
			//	try Mixed
			vp = D3DCREATE_MIXED_VERTEXPROCESSING;
			hr = _d3d9->CreateDevice(adapter, deviceType, _hwnd, vp, &_presentParas, &_device);

			if (FAILED(hr))
			{
				//	try Software
				vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				hr = _d3d9->CreateDevice(adapter, deviceType, Paras::getInstancePtr()->_hwnd, vp, &_presentParas, &_device);

				if (FAILED(hr))
				{
					//	give up :(
					Error((hr));
					
					return false;
				}
			}
		}

		//
		_initVertexDeclaration();

		//
// 		_camera = new Camera;
// 		Paras::getInstancePtr()->_camera = _camera;
		
		//
		Paras::getInstancePtr()->_device = _device;

		//
		//switchProjectionMode();

		//
		return true;
	}

	void RenderSystem::_initPresentParas()
	{
		//
		RECT rect;
		GetClientRect(_hwnd, &rect);

		ZeroMemory(&_presentParas, sizeof(_presentParas));
		// 
		_presentParas.BackBufferCount			= 1;

		// The back buffer format. 
		_presentParas.BackBufferFormat			= D3DFMT_A8R8G8B8;

		// Width of the new swap chain's back buffers, in pixels
		_presentParas.BackBufferWidth			= rect.right - rect.left;

		// Height of the new swap chain's back buffers, in pixels. 
		_presentParas.BackBufferHeight			= rect.bottom - rect.top;

		// Multisampling is supported only if the swap effect is D3DSWAPEFFECT_DISCARD.
		_presentParas.MultiSampleType			= D3DMULTISAMPLE_NONE;
		
		// Quality level.
		_presentParas.MultiSampleQuality		= 0;
		
		// If this value is TRUE, Direct3D will manage depth buffers for the application
		_presentParas.EnableAutoDepthStencil	= true;
		
		//
		_presentParas.AutoDepthStencilFormat	= D3DFMT_D16;

		// TRUE if the application runs windowed; FALSE if the application runs full-screen.
		_presentParas.Windowed					= true;

		//
		_presentParas.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		
		// The device window determines the location and size of the back buffer on screen.  
		_presentParas.hDeviceWindow				= _hwnd;

		//
		_presentParas.Flags						= 0;
		
		// The rate at which the display adapter refreshes the screen. 
		_presentParas.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
// 		DEVMODE devMode;     
// 		::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
// 		_presentParas.FullScreen_RefreshRateInHz = devMode.dmDisplayFrequency; 
// 		
		// The maximum rate at which the swap chain's back buffers can be presented to the front buffer. 
		//_presentParas.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;
		_presentParas.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	void RenderSystem::destroy()
	{
		//
		//
		for (unsigned char i = VD_NULL; i != VD_COUNT; ++i)
		{
			_vertexDeclaration[i]->Release();
			_vertexDeclaration[i] = NULL;
		}

		// ... ? how to release shader ?

		//
		if (_device)
		{
			_device->Release();
			_device = NULL;
		}

		//
		if (_d3d9)
		{
			_d3d9->Release();
			_d3d9 = NULL;
		}

		//
// 		if (_camera)
// 		{
// 			delete _camera;
// 			_camera = NULL;
// 		}
	}

	void RenderSystem::_initVertexDeclaration()
	{
		//
		D3DVERTEXELEMENT9 ve[VD_COUNT][4] = 
		{
			//	VD_NULL
			{
				D3DDECL_END()
			},

			//	VD_POSITION_COLOR
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				D3DDECL_END()
			},

			//	VD_POSITION_COLOR
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				D3DDECL_END()
			},

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
			//	VD_POSITION_TEXTURE
			{
				{0,		0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{0,		12,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	VD_TPOSITION_TEXTURE
			{
				{0,		0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
				{0,		16,	D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	VD_POSITION_COLOR_TEXTURE
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				{ 0,	16, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	VD_TPOSITION_COLOR_TEXTURE
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},
				{ 0,	16, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},
				{ 0,	20, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},

			//	VD_POSITION_NORMAL_TEXTURE
			{
				{ 0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
				{ 0,	12, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
				{ 0,	24, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
				D3DDECL_END()
			},
		};
		
		//
		for (unsigned char i = VD_NULL; i != VD_COUNT; ++i)
		{
			_device->CreateVertexDeclaration(ve[i], _vertexDeclaration + i);
		}
	}

	void RenderSystem::beginFrame( bool clearColorBuffer /*= true*/, bool clearDepthBuffer /*= true*/, bool clearStencilBuffer /*= false*/ )
 	{
// 		D3DVIEWPORT9 pViewport;
// 		getViewport(&pViewport);
// 		RECT rt;
// 		rt.left = pViewport.X;
// 		rt.right = pViewport.Width;
// 		rt.top = pViewport.Y;
// 		rt.bottom = pViewport.Height;
		//_device->SetScissorRect(&rt);
		if (_device == NULL)
		{
			return;
		}
		DWORD flag = 0;
		if (clearColorBuffer)
		{
			flag |= D3DCLEAR_TARGET;
		}
		if (clearDepthBuffer)
		{
			flag |= D3DCLEAR_ZBUFFER;
		}
		if (clearStencilBuffer)
		{
			flag |= D3DCLEAR_STENCIL;
		}

		// Clear the back buffer
		_device->Clear(0, NULL, flag, _clearColor.getARGB(), 1.0f, 0L);

		// Begin the scene
		_device->BeginScene();
	}

	void RenderSystem::endFrame()
	{
		// End the scene
		_device->EndScene();

		// present the back buffer to the front buffer
		HRESULT hr = _device->Present(NULL, NULL, NULL, NULL);
		
		if (hr == D3DERR_DEVICELOST)
		{
			Error("D3DERR_DEVICELOST");

			if(_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			{
				onLostDevice();
				onResetDevice();
			}
		}
	}

	bool RenderSystem::queryShaderCaps( unsigned int major, unsigned int minor )
	{
		//
		if (_caps.VertexShaderVersion < D3DVS_VERSION(major, minor) ||
			_caps.PixelShaderVersion < D3DPS_VERSION(major, minor))
		{
			return false;
		}

		//
		return true;
	}

	void RenderSystem::drawPoint( Vec3 point, RGBA color )
	{
		POSITION_COLOR v[] =
		{
			POSITION_COLOR(point, color),
		};

		setVertexDeclaration(VD_POSITION_COLOR);
		_device->DrawPrimitiveUP(D3DPT_POINTLIST,1, (const void*)v, sizeof(POSITION_COLOR));
		setVertexDeclaration(VD_NULL);
	}

	void RenderSystem::drawLine(Vec3 beginPoint, Vec3 endPoint, RGBA color)
	{
		POSITION_COLOR v[] =
		{
			POSITION_COLOR(beginPoint, color),
			POSITION_COLOR(endPoint, color)
		};
		setVertexDeclaration(VD_POSITION_COLOR);
		_device->DrawPrimitiveUP(D3DPT_LINELIST,1, (const void*)v, sizeof(POSITION_COLOR));
		setVertexDeclaration(VD_NULL);
	}

	bool RenderSystem::setVertexDeclaration( eVertexDeclarationType vd )
	{
		HRESULT hr = _device->SetVertexDeclaration(_vertexDeclaration[vd]);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setViewport( const D3DVIEWPORT9 * pViewport )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetViewport(pViewport);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}


	bool RenderSystem::getViewport( D3DVIEWPORT9 * pViewport )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->GetViewport(pViewport);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}


	bool RenderSystem::setWorldMatrix( const D3DMATRIX * pMatrix )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetTransform(D3DTS_WORLD, pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setWorldMatrix( const Mat4& pM )
	{
		if (NULL == _device)
		{
			return false;
		}

		
		D3DXMATRIX pMatrix = D3D9Mapping::makeD3DXMatrix(pM);
	
		//
		HRESULT hr = _device->SetTransform(D3DTS_WORLD, &pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}
	bool RenderSystem::setViewMatrix( const D3DMATRIX * pMatrix )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		//
		HRESULT hr = _device->SetTransform(D3DTS_VIEW, pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setViewMatrix( const Mat4& pM )
	{
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(pM);
		return setViewMatrix(&m);
	}

	bool RenderSystem::setProjectionMatrix( const D3DMATRIX * pMatrix )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		//
		HRESULT hr = _device->SetTransform(D3DTS_PROJECTION, pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setProjectionMatrix( const Mat4& pM )
	{
		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(pM);
		return setProjectionMatrix(&m);
	}

	bool RenderSystem::update(u32 current, u32 delta)
	{
// 		Mat4 viewMat = Paras::getInstancePtr()->_Camera->getViewMatrix();
// 		Mat4 controllerMat = Mat4::IDENTITY;
// 		if (Paras::getInstancePtr()->_Camera->getControllerThirdPerson())
// 		{
// 			controllerMat = Paras::getInstancePtr()->_Camera->getControllerThirdPerson()->getMatrix();
// 		}
// 
// 		Mat4 projectMat = Paras::getInstancePtr()->_Camera->getProjectionMatrix();
// 
// 		Paras::getInstancePtr()->_renderSystem->setViewMatrix(viewMat * controllerMat);
// 		Paras::getInstancePtr()->_renderSystem->setProjectionMatrix(projectMat);
		//
		return true;
	}

	// A device can have up to eight set textures, so the maximum value here is 7.
	bool RenderSystem::setTexture( unsigned short index, ITexture* tex )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		if (NULL == tex)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetTexture(index, tex->getTexture());
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::drawPrimitive( D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::drawPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::drawIndexedPrimitive( D3DPRIMITIVETYPE Type, int BaseVertexIndex, unsigned int MinIndex, unsigned int NumVertices, unsigned int StartIndex, unsigned int PrimitiveCount )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->DrawIndexedPrimitive(Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::drawIndexedPrimitiveUP( D3DPRIMITIVETYPE PrimitiveType, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int PrimitiveCount, const void * pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::getWorldMatrix( D3DMATRIX * pMatrix )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->GetTransform(D3DTS_WORLD, pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setTextureStageState( unsigned long Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned long Value )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetTextureStageState( Stage, Type, Value );
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setRenderState( D3DRENDERSTATETYPE State, unsigned long Value )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetRenderState( State, Value );
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setSamplerState( unsigned long Sampler, D3DSAMPLERSTATETYPE Type, unsigned long Value )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetSamplerState( Sampler, Type, Value );
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setLight( unsigned int index, Light* light )
	{
		if (NULL == _device)
		{
			return false;
		}

		if (NULL == light)
		{
			_device->LightEnable(index, false);
			return true;
		}

		//
		D3DLIGHT9* lig = NULL;
		lig = light->getLight();
		if (NULL == lig)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetLight( index, lig);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		hr = _device->LightEnable( index, true);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setMaterial( Material* material )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		if (NULL == material)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetMaterial(material->getMaterial());
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setStreamSource( unsigned int StreamNumber, VertexBuffer* pStreamData, unsigned int OffsetInBytes, unsigned int Stride )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetStreamSource( StreamNumber, pStreamData->getVertexBuffer(), OffsetInBytes, Stride );
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	bool RenderSystem::setIndices( IndexBuffer* pIndexData )
	{
		if (NULL == _device)
		{
			return false;
		}

		//
		HRESULT hr = _device->SetIndices(pIndexData->getIndexBuffer());
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

// 	void RenderSystem::switchProjectionMode()
// 	{
// 		D3DXMATRIX vm;
// 
// 		_3DMode = !_3DMode;
// 		//
// 		if (_3DMode)
// 		{
// 			//
// 			D3DXMatrixPerspectiveFovRH(&vm, D3DX_PI * 0.45, 1.0f, 0.001f, 10000.f);
// 		}
// 		else
// 		{
// 			//
// 			D3DXMatrixOrthoRH(&vm, _presentParas.BackBufferWidth, _presentParas.BackBufferHeight, 1.0f, 1000.0f);
// 		}
// 
// 		//
// 		setProjectionMatrix(&vm);
// 	}

// 	bool RenderSystem::isPerspectiveProjectionMode()
// 	{
// 		return _3DMode;
// 	}
// 
// 	bool RenderSystem::isOrthographicProjectionMode()
// 	{
// 		return !_3DMode;
// 	}

	void RenderSystem::resetCamera()
	{
// 		if (_camera)
// 		{
// 			_camera->reset();
// 		}
	}

	void RenderSystem::resetDevice()
	{

	}

	void RenderSystem::drawQuad( const POSITION_TEXTURE* v, ITexture* tex )
	{
		//
		static unsigned short i[6] = {0, 1, 2, 0, 2, 3};
		
		setVertexDeclaration(VD_POSITION_TEXTURE);
		
		setTexture(0, tex);

		//
		drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, i, 
			D3DFMT_INDEX16, v, sizeof(POSITION_TEXTURE));

		//
		setTexture(0, 0);
	}

	void RenderSystem::drawQuad( const TPOSITION_TEXTURE* v, ITexture* tex )
	{
		//
		static unsigned short i[6] = {0, 1, 2, 0, 2, 3};

		setVertexDeclaration(VD_TPOSITION_TEXTURE);

		setTexture(0, tex);

		//
		drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, i, 
			D3DFMT_INDEX16, v, sizeof(TPOSITION_TEXTURE));

		//
		setTexture(0, 0);
	}

	bool RenderSystem::setTextureMatrix( u32 index, const Mat4& m )
	{
		if (NULL == _device)
		{
			return false;
		}


		D3DXMATRIX pMatrix = D3D9Mapping::makeD3DXMatrix(m);

		//
		HRESULT hr = _device->SetTransform(D3DTRANSFORMSTATETYPE(D3DTS_TEXTURE0 + index), &pMatrix);
		if (FAILED(hr))
		{
			Error((hr));
			return false;
		}

		//
		return true;
	}

	Ray RenderSystem::getPickingRay( Vec2& point, Mat4& mProj)
	{
		D3DVIEWPORT9 vp;
		_device->GetViewport(&vp);

		D3DXMATRIX proj;
		//_device->GetTransform(D3DTS_PROJECTION, &proj);
		//Mat4 mProj = Paras::getInstance()._Camera->getProjectionMatrix();
		proj = D3D9Mapping::makeD3DXMatrix(mProj);

		D3DXVECTOR3 origin;
		origin.x = 0.0f;
		origin.y = 0.0f;
		origin.z = 0.0f;
		D3DXVECTOR3 direction;
		direction.x = (((2.0f * point.x) / vp.Width) - 1.0f) / proj(0, 0);
		direction.y = (((-2.0f * point.y) / vp.Height) + 1.0f) / proj(1, 1);
		direction.z = 1.0f;
		// until here, we got a ray in the view space
		
		// next, we need to transform it into the world space
		D3DXMATRIX view;
		//_device->GetTransform(D3DTS_VIEW, &view);
		Mat4 mView = Paras::getInstance()._Camera->getViewMatrix();
		view = D3D9Mapping::makeD3DXMatrix(mView);

		D3DXMATRIX viewInverse;
		D3DXMatrixInverse(&viewInverse, NULL, &view);

		D3DXVec3TransformCoord(&origin, &origin, &viewInverse);
	
		D3DXVec3TransformNormal(&direction, &direction, &viewInverse);
		D3DXVec3Normalize(&direction, &direction);

		Ray r;
		r._direction.x = direction.x;
		r._direction.y = direction.y;
		r._direction.z = -direction.z;
		r._origin.x = origin.x;
		r._origin.y = origin.y;
		r._origin.z = origin.z;

		return r;
	}

	LPDIRECT3DDEVICE9 RenderSystem::getDevice()
	{
		return _device;
	}

	// D3DPOOL_DEFAULT加载的资源释放掉
	void RenderSystem::onLostDevice()
	{

	}

	// This is the best location to create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever the device is lost
	void RenderSystem::onResetDevice()
	{
		// Reset函数是设备丢失后设备对象(LPDIRECT3DDEVICE9 device)唯一可以调用的函数
		if(FAILED(_device->Reset(&_presentParas)))
		{
			return;
		}
	}

	void RenderSystem::setVertexShader( IShader* s )
	{
		if (s != NULL)
		{
			_device->SetVertexShader(s->getVertexShader());
		}
		else
		{
			_device->SetVertexShader(NULL);
		}
	}

	void RenderSystem::setPixelShader( IShader* s )
	{
		if (s != NULL)
		{
			_device->SetPixelShader(s->getPixelShader());
		}
		else
		{
			_device->SetPixelShader(NULL);
		}
	}

	void RenderSystem::setClearColor( Color& c )
	{
		_clearColor = c;
	}

}