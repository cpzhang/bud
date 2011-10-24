//==========================================================================
/**
* @file	  : VertexShader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   12:38
* purpose : 
*/
//==========================================================================

#ifndef __VertexShader_h__
#define __VertexShader_h__

#include "Common.h"
#include "IShader.h"
namespace Euclid
{
	class VertexShader : public IShader
	{
	public:
		VertexShader(const std::string& name, const std::string& fileName);
		~VertexShader();

		//
	public:
		virtual bool createShader();

	};
}

#endif // __VertexShader_h__
