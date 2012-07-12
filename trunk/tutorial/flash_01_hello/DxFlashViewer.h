#pragma once
#include <d3dx9.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#pragma comment(lib,"atl")

//#import "flash.ocx"
using namespace ShockwaveFlashObjects; 


class DxFlashViewer
{
public:
	DxFlashViewer(void);
	~DxFlashViewer(void);

	bool Init( IDirect3DDevice9* pDevice, HWND hWnd, int width, int height );

	void OpenFlash(const char* filename);

	void UpdateTexture();

	IDirect3DTexture9* GetTexturePtr();

	int GetViewerWidth() const;
	int GetViewerHeight() const;
	int GetTextureWidth() const;
	int GetTextureHeight() const;

	void OnWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	int mViewerWidth;
	int mViewerHeight;
	int mTextureWidth;
	int mTextureHeight;
	IDirect3DTexture9* mTexture;
	IDirect3DDevice9* mDevice;

	HWND mViewerWnd;
	IShockwaveFlash* mFlashCtrl;
	IOleInPlaceObjectWindowless* mWndlessObject;
};

