//==========================================================================
/**
* @file	  : IShader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   12:35
* purpose : 
*/
//==========================================================================

#ifndef __IShader_h__
#define __IShader_h__

#include "stdafx.h"
#include "D3D9Mapping.h"
#include "Paras.h"
#include "Camera.h"

namespace Euclid
{
	class _EuclidExport_ IShader
	{
	public:
		IShader(const std::string& name, const std::string& fileName)
			: _entryPoint("main"), _profile("vs_2_0"), _shader(0), _error(0), _table(0),
			_vs(0), _ps(0), _name(name), _fileName(fileName)
		{
			_device = Paras::getInstancePtr()->_device;
		}
		virtual ~IShader()
		{
			destroy();
		}
		
		//
		void destroy()
		{
			if (_vs)
			{
				_vs->Release();
				_vs = NULL;
			}

			if (_ps)
			{
				_ps->Release();
				_ps = NULL;
			}

			if (_shader)
			{
				_shader->Release();
				_shader = NULL;
			}

			if (_table)
			{
				_table->Release();
				_table = NULL;
			}

			if (_error)
			{
				_error->Release();
				_error = NULL;
			}
		}

		bool compileShaderFromFile(unsigned long flag)
		{
			DWORD dwShaderFlags = flag;

#if defined( DEBUG ) || defined( _DEBUG )
			// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
			// Setting this flag improves the shader debugging experience, but still allows 
			// the shaders to be optimized and to run exactly the way they will run in 
			// the release configuration of this program.
			dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
			dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
#endif
#ifdef DEBUG_PS
			dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
#endif

			HRESULT hr;
			if (FAILED(hr = D3DXCompileShaderFromFile(_fileName.c_str(), NULL, NULL, getEntryPoint().c_str(), getProfile().c_str(), dwShaderFlags, &_shader,	NULL, &_table)))
			{
				std::ostringstream buf;
				buf<<"D3DXCreateTextureFromFile "<<_fileName<<" Error Code : "<<(hr); 
				Error(buf.str());
				return false;
			}

			return true;
		}

	public:
		virtual bool createShader() = 0;

	public:
		IDirect3DVertexShader9* getVertexShader()
		{
			return _vs;
		}
		IDirect3DPixelShader9* getPixelShader()
		{
			return _ps;
		}
		void setEntryPoint(const std::string& s)
		{
			_entryPoint = s;
		}
		std::string getEntryPoint()
		{
			return _entryPoint;
		}
		void setProfile(const std::string& s)
		{
			_profile = s;
		}
		std::string getProfile()
		{
			return _profile;
		}

		bool setMatrix(const std::string& varialbe, const Mat4& mat)
		{
			return _table->SetMatrix(_device, varialbe.c_str(), &D3D9Mapping::makeD3DXMatrix(mat));
		}

		bool setFloat(const std::string& v, float f)
		{
			return _table->SetFloat(_device, v.c_str(), f);
		}

		// proteced, not private because we will inherit this class
	protected:
		std::string				_name;
		std::string				_fileName;
		std::string				_entryPoint;
		std::string				_profile;
		LPD3DXBUFFER			_shader;
		LPD3DXBUFFER			_error;
		LPD3DXCONSTANTTABLE		_table;
		IDirect3DDevice9*		_device;
		IDirect3DVertexShader9* _vs;
		IDirect3DPixelShader9*	_ps;
	};
}

#endif // __IShader_h__
