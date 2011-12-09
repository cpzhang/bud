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

	void Effect::setMatrix( const std::string& name, Mat4 *pMatrix )
	{
		_effect->SetMatrix(name.c_str(), &DXMapping::toDXMatrix(*pMatrix));
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
		// Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
		// shader debugger. Debugging vertex shaders requires either REF or software vertex 
		// processing, and debugging pixel shaders requires REF.  The 
		// D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
		// shader debugger.  It enables source level debugging, prevents instruction 
		// reordering, prevents dead code elimination, and forces the compiler to compile 
		// against the next higher available software target, which ensures that the 
		// unoptimized shaders do not exceed the shader model limitations.  Setting these 
		// flags will cause slower rendering since the shaders will be unoptimized and 
		// forced into software.  See the DirectX documentation for more information about 
		// using the shader debugger.
		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

		// Preshaders are parts of the shader that the effect system pulls out of the 
		// shader and runs on the host CPU. They should be used if you are GPU limited. 
		// The D3DXSHADER_NO_PRESHADER flag disables preshaders.
		//if( !g_bEnablePreshader )
		//	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

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