#include "Texture.h" 
#include "TextureManager.h"
#include "RenderSystem.h"
#include "tool/flash/IFlashDX.h"
namespace Euclid
{
	Texture::Texture()
	{
		_texture = 0;
		_references = 0;
	}

	Texture::~Texture()
	{
		destroy();
	}

	IDirect3DTexture9* Texture::getTexture()
	{
		return _texture;
	}

	bool Texture::loadFromFile( const tstring& fileName )
	{
		HRESULT hr = D3DXCreateTextureFromFile(RenderSystem::getInstancePtr()->getDevice(),
			fileName.c_str(), &_texture);
		if (FAILED(hr))
		{
			tstring dataFile;
			Buddha::FileSystem::getInstancePtr()->getDataDirectory(dataFile);
			dataFile += TEXT("\\");
			//
			tstring path(fileName);
			path = dataFile + fileName;
			HRESULT hr = D3DXCreateTextureFromFileEx(RenderSystem::getInstancePtr()->getDevice(), path.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &_texture);
			if (FAILED(hr))
			{
				throw EDX(hr);
			}
		}

		return true;
	}

	bool Texture::loadFromMemory( unsigned char* buffer, unsigned int length )
	{
		HRESULT hr;
		hr = D3DXCreateTextureFromFileInMemory(_device, (LPCVOID)buffer, length, &_texture);
		if (FAILED(hr))
		{
			return false;
		}
		return true;
	}

	bool Texture::loadFromRawData( unsigned char* buffer, unsigned int width, unsigned int height, unsigned int pitch, D3DFORMAT format )
	{
		HRESULT hr;
		//
		hr = D3DXCreateTexture(RenderSystem::getInstancePtr()->getDevice(), width, height, 1, 0, format, D3DPOOL_MANAGED, &_texture);
		if (FAILED(hr))
		{
			return false;
		}
		//
		IDirect3DSurface9* sur = 0;
		_texture->GetSurfaceLevel(0, &sur);
		if (sur)
		{
			RECT rc = {0, 0, width, height};
			//
			hr = D3DXLoadSurfaceFromMemory(sur, 0, NULL, (void*)buffer, format, pitch, 0, &rc, D3DX_DEFAULT, 0);
			//
			sur->Release();
			//
			/*if (buffer)
			{
				delete buffer;
				buffer = 0;
			}*/
		}
		return true;
	}

	bool Texture::createEmpty(u32 Width, u32 Height, u32 Levels, eUsage Usage, eFormat Format, ePool Pool)
	{
		// The first two parameters that are accepted by IDirect3DDevice9::CreateTexture are the size and width of the top-level texture. 
		// The third parameter specifies the number of levels in the texture. If you set this to zero, Direct3D creates a chain of surfaces,
		// each a power of two smaller than the previous one, down to the smallest possible size of 1x1. 
		// The fourth parameter specifies the usage for this resource; in this case, 0 is specified to indicate no specific usage for the resource. 
		// The fifth parameter specifies the surface format for the texture. Use a value from the D3DFORMAT enumerated type for this parameter.
		// The sixth parameter specifies a member of the D3DPOOL enumerated type indicating the memory class into which to place the created resource.
		// Unless you are using dynamic textures, D3DPOOL_MANAGED is recommended.
		// The final parameter takes the address of a pointer to an IDirect3DTexture9 interface.
		HRESULT hr = D3DXCreateTexture(RenderSystem::getInstancePtr()->getDevice(), Width, Height, Levels, Usage,
			(D3DFORMAT)Format, (D3DPOOL)Pool, &_texture);

		if(FAILED(hr))
		{
			throw EDX(hr);
		}

		//
		return true;
	}

	bool Texture::setSubData( unsigned int level, unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int pitch, void* pData, D3DFORMAT pf )
	{
		if(NULL == _texture)
		{
			return false;
		}

		//
		IDirect3DSurface9 *pSurface = 0;
		_texture->GetSurfaceLevel(0, &pSurface);
		if (NULL == pSurface)
		{
			return false;
		}

		//
		RECT rc = {0, 0, width, height};
		RECT drc = {left, top, width + left, height + top};
		
		//
		HRESULT hr = D3DXLoadSurfaceFromMemory(pSurface, 0, &drc, pData, pf,
			pitch, 0, &rc, D3DX_DEFAULT, 0);
		if(FAILED(hr))
		{
			return false;
		}
		pSurface->Release();

		//
		return false;
	}

	void Texture::destroy()
	{
		if (_texture)
		{
			_texture->Release();
			_texture = NULL;
		}
	}

	void Texture::release()
	{
		if (_references > 0)
		{
			--_references;
		}
		if (_references == 0)
		{
			destroy();
			TextureManager::getInstancePtr()->onReleaseTexture(this);
			delete this;
		}
	}

	void Texture::addReference()
	{
		++_references;
	}

	void Texture::onInvalidateDevice()
	{

	}

	void Texture::onRestoreDevice()
	{

	}

	bool Texture::update( IFlashDXPlayer* p )
	{
		IDirect3DSurface9* pSrcSurface;
		HRESULT hr = _texture->GetSurfaceLevel(0, &pSrcSurface);

		HDC surfaceDC;
		hr = pSrcSurface->GetDC(&surfaceDC);
		assert(SUCCEEDED(hr));

		// Draw flash frame
		p->DrawFrame(surfaceDC);

		hr = pSrcSurface->ReleaseDC(surfaceDC);

		//RenderSystem::getInstancePtr()->getDevice()->UpdateTexture()		
		pSrcSurface->Release();
		
		return true;
	}

}