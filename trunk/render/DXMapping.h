//==========================================================================
/**
* @file	  : D3D9Mapping.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-13   20:06
* purpose : 
*/
//==========================================================================

#ifndef __D3D9Mapping_h__
#define __D3D9Mapping_h__

#include "Common.h"

//
namespace Euclid
{
	class DXMapping
	{
	public:
		// Convert matrix to D3D style
		static 	D3DXMATRIX toDXMatrix( const Mat4& mat );
		// Convert matrix from D3D style
		static Mat4 fromDXMatrix( const D3DXMATRIX& mat );
	};
}

#endif // __D3D9Mapping_h__
 
