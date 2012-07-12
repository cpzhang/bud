#include "StdAfx.h"
#include "DxFlashViewer.h"


DxFlashViewer::DxFlashViewer(void)
: mViewerWidth(0)
, mViewerHeight(0)
, mTextureWidth(0)
, mTextureHeight(0)
, mTexture(NULL)
, mDevice(NULL)
, mViewerWnd(0)
, mFlashCtrl(NULL)
, mWndlessObject(NULL)
{
}

DxFlashViewer::~DxFlashViewer(void)
{
	DestroyWindow(this->mViewerWnd);
	if (NULL != this->mTexture)
	{
		this->mTexture->Release();
		this->mTexture = NULL;
	}
	if (NULL != this->mFlashCtrl)
	{
		this->mFlashCtrl->Release();
		this->mFlashCtrl = NULL;
	}
	if (NULL != this->mWndlessObject)
	{
		this->mWndlessObject->Release();
		this->mWndlessObject = NULL;
	}
}

bool DxFlashViewer::Init( IDirect3DDevice9* pDevice, HWND hWnd, int width, int height )
{
	if (NULL == pDevice || width <= 0 || height <= 0)
		return false;

	this->mDevice = pDevice;
	this->mViewerWidth = width;
	this->mViewerHeight = height;

	// calculate texture size required (next power of two above browser window size)
	for ( mTextureWidth = 1; mTextureWidth < mViewerWidth; mTextureWidth <<= 1 );
	for ( mTextureHeight = 1; mTextureHeight < mViewerHeight; mTextureHeight <<= 1 );

	// create the browser window in the background
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = "FlashViewer";
	RegisterClassEx(&wc);

	this->mViewerWnd = CreateWindowEx(0, "FlashViewer", "FlashViewer", WS_POPUP
		, 0, 0, width, height, hWnd, NULL, NULL, 0);

	if (!this->mViewerWnd)
		return false;

	//embed the player in the window
	HRESULT hr;
	hr = CoCreateInstance(__uuidof(ShockwaveFlash), 0, CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&(this->mFlashCtrl)); 
	if (FAILED(hr))
		return false;
	hr = this->mFlashCtrl->put_WMode(L"transparent");
	hr = AtlAxAttachControl(this->mFlashCtrl, this->mViewerWnd, NULL);
	if (FAILED(hr))
		return false;
	hr = this->mFlashCtrl->QueryInterface(__uuidof(IOleInPlaceObjectWindowless), (void**)&this->mWndlessObject);
	if (FAILED(hr))
		return false;

	// create the render texture for Direct3D
	hr =  this->mDevice->CreateTexture(this->mTextureWidth, this->mTextureHeight
		, 0, NULL, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &this->mTexture, NULL);

	if (FAILED(hr))
		return false;

	return true;
}

void DxFlashViewer::OpenFlash( const char* filename )
{
	this->mFlashCtrl->put_Movie(_bstr_t(filename));
}

IDirect3DTexture9* DxFlashViewer::GetTexturePtr()
{
	return this->mTexture;
}

int DxFlashViewer::GetViewerWidth() const
{
	return this->mViewerWidth;
}

int DxFlashViewer::GetViewerHeight() const
{
	return this->mViewerHeight;
}

int DxFlashViewer::GetTextureWidth() const
{
	return this->mTextureWidth;
}

int DxFlashViewer::GetTextureHeight() const
{
	return this->mTextureHeight;
}

void DxFlashViewer::UpdateTexture()
{
	if(NULL == this->mViewerWnd)
		return;
	RECT rect = {0, 0, this->mViewerWidth, this->mViewerHeight};
	IDirect3DSurface9* pSurface = NULL;
	this->mTexture->GetSurfaceLevel(0, &pSurface);
	if (NULL != pSurface)
	{
		HDC hdcTexture;
		HRESULT hr = pSurface->GetDC(&hdcTexture);
		if(FAILED(hr)) return;
		::SetMapMode(hdcTexture, MM_TEXT);
		::OleDraw(this->mFlashCtrl, DVASPECT_CONTENT, hdcTexture, &rect);
		pSurface->ReleaseDC(hdcTexture);
		pSurface->Release();
	}
}

void DxFlashViewer::OnWindowMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr;
	if (NULL != mWndlessObject)
	{
		mWndlessObject->OnWindowMessage(msg, wParam, lParam, &lr);
	}
}