/********************************************************************
**	file: 		IShaderFactory.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-20
**	purpose:	
*********************************************************************/
#ifndef __IShaderFactory_h__
#define __IShaderFactory_h__

#include "Common.h"

class VertexShader;
class PixelShader;

namespace Euclid
{
	class IShaderFactory
	{
	public:
		//
		virtual IShader* createVertexShader(const std::string& name, const std::string& fileName) = NULL;

		//
		virtual IShader* createPixelShader(const std::string& name, const std::string& fileName) = NULL;
	};
}
#endif // __IShaderFactory_h__