#include "PixelShader.h"

namespace Euclid
{
	PixelShader::PixelShader(const std::string& name, const std::string& fileName)
		:IShader(name, fileName)
	{

	}

	PixelShader::~PixelShader()
	{

	}

	bool PixelShader::createShader()
	{
		if (FAILED(_device->CreatePixelShader(( DWORD* )_shader->GetBufferPointer(), &_ps)))
		{
			return false;
		}
		
		return true;
	}
}
