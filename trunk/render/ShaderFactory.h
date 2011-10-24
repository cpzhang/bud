/********************************************************************
**	file: 		ShaderFactory.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-20
**	purpose:	
*********************************************************************/
#ifndef __ShaderFactory_h__
#define __ShaderFactory_h__

#include "IShaderFactory.h"

namespace Euclid
{
	class ShaderFactory : public IShaderFactory, public Buddha::SingletonEx<ShaderFactory>
	{
	public:
		ShaderFactory();
		~ShaderFactory();
	public:
		//
		virtual IShader* createVertexShader(const std::string& name, const std::string& fileName);

		//
		virtual IShader* createPixelShader(const std::string& name, const std::string& fileName);
	};
}

#endif // __ShaderFactory_h__
