#include "Effect.h"
#include "DXMapping.h"
#include "RenderSystem.h"
#include "ITexture.h"
#include "EffectManager.h"
//
namespace Zen
{
	Effect::~Effect()
	{
	}

	void Effect::destroy()
	{
		if (_references > 0)
		{
			--_references;
		}
		if (_references == 0)
		{
			if (_effect)
			{
				_effect->Release();
				_effect = NULL;
			}
			EffectManager::getInstancePtr()->onDestroyEffect(this);
		}
	}

	bool Effect::begin( u32* count )
	{
		return SUCCEEDED(_effect->Begin(count, D3DXFX_DONOTSAVESTATE));
	}

	void Effect::end()
	{
		_effect->End();
	}

	bool Effect::beginPass( u32 index )
	{
		return SUCCEEDED(_effect->BeginPass(index));
	}

	void Effect::endPass()
	{
		_effect->EndPass();
	}

	void Effect::onLostDevice()
	{
		if (_effect)
		{
			_effect->OnLostDevice();
		}
	}

	void Effect::onResetDevice()
	{
		if (_effect)
		{
			HRESULT hr = _effect->OnResetDevice();
			if (S_OK != hr)
			{
				throw EDX(hr);
			}
			
		}
	}

	void Effect::setTexture( const std::string& name, ITexture *pTexture )
	{
		NameTextureMap::iterator it = _nameTextures.find(name);
		if (it == _nameTextures.end())
		{
			_nameTextures[name] = pTexture;
		}
		else
		{
			if (it->second == pTexture)
			{
				return;
			}
		}

		_nameTextures[name] = pTexture;
		if (pTexture)
		{
			HRESULT r = _effect->SetTexture(name.c_str(), pTexture->getTexture());
			if (FAILED(r))
			{
				throw EDX(r);
			}
		}
	}

	void Effect::setMatrix( const std::string& name, const Mat4 *pMatrix )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetMatrixTranspose(name.c_str(), (D3DXMATRIX*)(pMatrix));
	}

	void Effect::setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		if (NULL == _effect)
		{
			return;
		}
		setMatrix(name, &pMatrix);
	}

	void Effect::setMatrixArray( const std::string& name, Mat4 *pMtxArray, u32 count )
	{
		// wooops!	
		if (NULL == _effect)
		{
			return;
		}
		HRESULT hr = _effect->SetMatrixTransposeArray(name.c_str(), (D3DXMATRIX*)(pMtxArray), count);
		if (FAILED(hr))
		{
			throw EDX(hr);
		}
	}

	void Effect::setTechnique( const std::string& name )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetTechnique(name.c_str());
	}

	void Effect::setFloatArray( const std::string& name, const float *pFloatArray, u32 count )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetFloatArray(name.c_str(), pFloatArray, count);
	}

	void Effect::commitChanges()
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->CommitChanges();
	}

	bool Effect::loadFromFile( const std::string& filename )
	{
		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
#define DEBUG
#ifdef DEBUG
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		// D3DXSHADER_DEBUG，打开后，可在pix里面查看源码，否则，汇编
		dwShaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION /*| D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT | D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT*/;
#endif

		// Create an effect from an ASCII or binary effect description
		//
		std::string data = filename;
		HRESULT r;
		LPD3DXBUFFER error;
		if (FAILED(r = D3DXCreateEffectFromFile(RenderSystem::getInstancePtr()->getDevice(), data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, &error)))
		{
			data.clear();
			Zen::FileSystem::getInstancePtr()->getDataDirectory(data);
			data += filename;
			if (FAILED(r = D3DXCreateEffectFromFile(RenderSystem::getInstancePtr()->getDevice(), data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, &error)))
			{
				throw EDX(r);
			}
		}
		_effectfFile = filename;

		return true;
	}

	bool Effect::loadFromMemory( const u8* data, u32 len )
	{
		DWORD dwFlag = 0;

		ID3DXBuffer *pError = NULL;
		HRESULT hr = D3DXCreateEffect(RenderSystem::getInstancePtr()->getDevice(), data, len, NULL, NULL, dwFlag, NULL, &_effect, &pError);
		if(FAILED(hr))
		{
			if(pError)
			{
				const char* szError = (const char*)pError->GetBufferPointer();
				std::ostringstream buf;
				buf<<"D3DXCreateEffect failed, reason:"<<szError;
				Error(buf.str().c_str());
			}
			else
			{
				Error("D3DXCreateEffect failed");
			}
			return false;
		}

		return true;
	}

	void Effect::setValue( const std::string& name, void* data, u32 bytes )
	{
		if (_effect)
		{
			_effect->SetValue(name.c_str(), data, bytes);
		}
	}

	void Effect::setInt( const std::string& name, int value )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetInt(name.c_str(), value);
	}

	void Effect::setFloat( const std::string& name, float value )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetFloat(name.c_str(), value);
	}

	void Effect::setBool( const std::string& name, bool value )
	{
		if (NULL == _effect)
		{
			return;
		}
		_effect->SetBool(name.c_str(), value);
	}

	bool Effect::getBool( const std::string& name )
	{
		if (NULL == _effect)
		{
			return false;
		}
		BOOL value;
		_effect->GetBool(name.c_str(), &value);
		return value;
	}

	bool Effect::setVector( const std::string& name, const Vec4* pVector )
	{
		if (NULL == _effect)
		{
			return false;
		}
		HRESULT r = _effect->SetVector(name.c_str(), (D3DXVECTOR4*)pVector);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	bool Effect::setVector( const std::string& name, Vec4* pVector )
	{
		if (NULL == _effect)
		{
			return false;
		}
		HRESULT r = _effect->SetVector(name.c_str(), (D3DXVECTOR4*)pVector);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	bool Effect::setVectorArray( const std::string& name, const Vec4* pVector, u32 Count )
	{
		if (NULL == _effect)
		{
			return false;
		}
		HRESULT r = _effect->SetVectorArray(name.c_str(), (D3DXVECTOR4*)pVector, Count);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	bool Effect::setVectorArray( const std::string& name, Vec4* pVector, u32 Count )
	{
		if (NULL == _effect)
		{
			return false;
		}
		HRESULT r = _effect->SetVectorArray(name.c_str(), (D3DXVECTOR4*)pVector, Count);
		if (D3D_OK == r)
		{
			return true;
		}

		return false;
	}

	void Effect::onInvalidateDevice()
	{
		if (_effect)
		{
			_effect->OnLostDevice();
			return;
		}
// 		if (_effect)
// 		{
// 			_effect->Release();
// 			_effect = NULL;
// 		}
// 		_nameTextures.clear();
	}

	void Effect::onRestoreDevice()
	{
		if (_effect)
		{
			_effect->OnResetDevice();
			return;
		}
//		loadFromFile(_effectfFile);
	}

	void Effect::addReference()
	{
		++_references;
	}

	void Effect::_clear()
	{
		_effect = NULL;
		_effectfFile.clear();
		_references = 0;
	}

	void Effect::getMatrix( const std::string& name, const Mat4 *pMatrix )
	{
		_effect->GetMatrixTranspose(name.c_str(), (D3DXMATRIX*)pMatrix);
	}
}