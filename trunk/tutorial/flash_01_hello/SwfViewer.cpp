//#include "stdafx.h"
#include <strsafe.h>
#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#pragma warning( default : 4996 ) 

#include "DxFlashViewer.h"

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	float x, y, z, rhw;	// The transformed position for the vertex
	float u, v;			// The texcoord
};
CUSTOMVERTEX ScreenQuadVertex[] =
{
	{   0.0f,   0.0f, 0.5f, 1.0f, 0.0f, 0.0f, }, // x, y, z, rhw, u, v
	{ 800.0f,   0.0f, 0.5f, 1.0f, 800.0f/1024, 0.0f, },
	{   0.0f, 600.0f, 0.5f, 1.0f, 0.0f, 600.0f/1024, },
	{ 800.0f, 600.0f, 0.5f, 1.0f, 800.0f/1024, 600.0f/1024, },
};
// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)

CComModule _Module;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
IDirect3DVertexBuffer9* g_pVertexBuffer = NULL;

DxFlashViewer* g_pViewer = NULL;


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Create the D3DDevice
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	// Create the vertex buffer.
	return g_pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX
		, D3DPOOL_DEFAULT, &(g_pVertexBuffer), NULL );
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (NULL != g_pViewer)
	{
		delete g_pViewer;
	}
	if( g_pd3dDevice != NULL ) 
		g_pd3dDevice->Release();

	if( g_pD3D != NULL )       
		g_pD3D->Release();
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Clear the backbuffer to a blue color
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	if (NULL != g_pViewer)
	{
		g_pViewer->UpdateTexture();
	}

	// Begin the scene
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{

		// Now we fill the vertex buffer.
		VOID* pVertexBuffer;
		if( FAILED(g_pVertexBuffer->Lock( 0, sizeof(ScreenQuadVertex), (void**)&pVertexBuffer, 0 ) ) )
			return;
		memcpy( pVertexBuffer, ScreenQuadVertex, sizeof(ScreenQuadVertex) );
		g_pVertexBuffer->Unlock();

		// use the browser texture
		g_pd3dDevice->SetTexture( 0, g_pViewer->GetTexturePtr() );
		// draw the single quad full screen (orthographic)
		g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );
		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_MOUSEACTIVATE:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		g_pViewer->OnWindowMessage(msg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		TEXT("DxFlashViewer"), NULL };
	RegisterClassEx( &wc );
	CoInitialize(NULL);  
	AtlAxWinInit();  

	// Create the application's window
	HWND hWnd = CreateWindow( TEXT("DxFlashViewer"), TEXT("DxFlashViewer"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL );

	// Initialize Direct3D
	if( SUCCEEDED(InitD3D(hWnd)) )
	{
		g_pViewer = new DxFlashViewer();
		// Show the window
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		if (!g_pViewer->Init(g_pd3dDevice, hWnd, 800, 600))
		{
			MessageBox(NULL, "Failed to init.", NULL, 0);
			return 0;
		}

		g_pViewer->OpenFlash("E:\\My Videos\\TestGame.swf");
		// Enter the message loop
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );
		while( msg.message!=WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				Render();
			}
		}
	}
	CoUninitialize();

	UnregisterClass( TEXT("DxFlashViewer"), wc.hInstance );
	return 0;
}

