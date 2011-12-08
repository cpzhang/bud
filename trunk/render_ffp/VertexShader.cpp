#include "stdafx.h"
#include "VertexShader.h"

//
namespace Euclid
{
	VertexShader::VertexShader(const std::string& name, const std::string& fileName)
		:IShader(name, fileName)
	{

	}

	VertexShader::~VertexShader()
	{

	}

	bool VertexShader::createShader()
	{
		if (FAILED(_device->CreateVertexShader(( DWORD* )_shader->GetBufferPointer(), &_vs)))
		{
			return false;
		}

		if (_shader)
		{
			_shader->Release();
			_shader = NULL;
		}

		return true;
	}
}
