#include "Effect.h"
#include "DXMapping.h"
#include "RenderSystem.h"
//
namespace Euclid
{
	Effect::~Effect()
	{
	}

	void Effect::destroy()
	{
		if (_effect)
		{
			_effect->Release();
			_effect = NULL;
		}
		delete this;
	}

	bool Effect::begin( u32* count )
	{
		return SUCCEEDED(_effect->Begin(count, 0));
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
			_effect->OnResetDevice();
		}
	}

// 	void Effect::setTexture( const std::string& name, ITexture *pTexture )
// 	{
// 		if (pTexture)
// 		{
// 			_effect->SetTexture(name.c_str(), pTexture->getTexture());
// 		}
// 		else
// 		{
// 			_effect->SetTexture(name.c_str(), NULL);
// 		}
// 	}

	void Effect::setMatrix( const std::string& name, const Mat4 *pMatrix )
	{
		_effect->SetMatrix(name.c_str(), &DXMapping::toDXMatrix(*pMatrix));
	}

	void Effect::setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		setMatrix(name, &pMatrix);
	}

	void Effect::setMatrixArray( const std::string& name, Mat4 *pMtxArray, u32 count )
	{
		// wooops!	
	}

	void Effect::setTechnique( const std::string& name )
	{
		_effect->SetTechnique(name.c_str());
	}

	void Effect::setFloatArray( const std::string& name, float *pFloatArray, u32 count )
	{
		_effect->SetFloatArray(name.c_str(), pFloatArray, count);
	}

	void Effect::commitChanges()
	{
		_effect->CommitChanges();
	}

	bool Effect::loadFromFile( const std::string& filename )
	{
		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG)
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif

		// Create an effect from an ASCII or binary effect description
		//
		std::string data = filename;
		HRESULT hr;
		if (FAILED(hr = D3DXCreateEffectFromFile(RenderSystem::getInstancePtr()->getDevice(), data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, NULL)))
		{
			data.clear();
			Buddha::FileSystem::getInstancePtr()->getDataDirectory(data);
			data += filename;
			if (FAILED(hr = D3DXCreateEffectFromFile(RenderSystem::getInstancePtr()->getDevice(), data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, NULL)))
			{
				data.clear();
				//
				char path[257];
				static const std::string tMaxProgramName("MaxPreview.exe");
				GetModuleFileName(GetModuleHandle(tMaxProgramName.c_str()), path, 256);
				std::string parentPath(path);
				parentPath.erase(parentPath.size() - tMaxProgramName.size(), tMaxProgramName.size());
				std::string previewProgramPath(parentPath);
				//
				data = previewProgramPath + filename;
				if (FAILED(hr = D3DXCreateEffectFromFile(RenderSystem::getInstancePtr()->getDevice(), data.c_str(), NULL, NULL, dwShaderFlags, NULL, &_effect, NULL)))
				{
					std::ostringstream buf;
					buf<<"D3DXCreateTextureFromFile "<<filename<<" Error Code : "<<(hr);
						Error(buf.str());
					return false;
				}
			}
		}

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
				Error(buf.str());
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
		_effect->SetInt(name.c_str(), value);
	}

	void Effect::setFloat( const std::string& name, float value )
	{
		_effect->SetFloat(name.c_str(), value);
	}

	void Effect::setBool( const std::string& name, bool value )
	{
		_effect->SetBool(name.c_str(), value);
	}

	bool Effect::getBool( const std::string& name )
	{
		BOOL value;
		_effect->GetBool(name.c_str(), &value);
		return value;
	}

}