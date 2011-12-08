/********************************************************************
**	file: 		PixelShader.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-20
**	purpose:	
*********************************************************************/
#ifndef __PixelShader_h__
#define __PixelShader_h__

#include "Common.h"
#include "IShader.h"

namespace Euclid
{
	class PixelShader : public IShader
	{
	public:
		PixelShader(const std::string& name, const std::string& fileName);
		~PixelShader();

	public:
		//
		virtual bool createShader();
	};
}
#endif // __PixelShader_h__
