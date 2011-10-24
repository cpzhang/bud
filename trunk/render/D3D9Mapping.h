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

#include "stdafx.h"
#include "Common.h"
#include "VertexDeclaration.h"

//
namespace Euclid
{
	class D3D9Mapping
	{
	public:
		// Convert matrix to D3D style
		static 	D3DXMATRIX makeD3DXMatrix( const Euler::Matrix4& mat );
		// Convert matrix from D3D style
		static Euler::Matrix4 convertD3DXMatrix( const D3DXMATRIX& mat );
		//
		static D3DPRIMITIVETYPE convert2D3DPrimitiveType(const ePrimitiveType t);
	};
}

#endif // __D3D9Mapping_h__
 
