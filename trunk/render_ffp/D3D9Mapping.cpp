#include "D3D9Mapping.h"

//
namespace Euclid
{
	//---------------------------------------------------------------------
	D3DXMATRIX D3D9Mapping::makeD3DXMatrix( const Euler::Matrix4& mat )
	{
		// Transpose matrix
		// D3D9 uses row vectors i.e. V*M
		// Ogre, OpenGL and everything else uses column vectors i.e. M*V
		return D3DXMATRIX(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
	}
	//---------------------------------------------------------------------
	Euler::Matrix4 D3D9Mapping::convertD3DXMatrix( const D3DXMATRIX& mat )
	{
		return Euler::Matrix4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
	}

	D3DPRIMITIVETYPE D3D9Mapping::convert2D3DPrimitiveType( const ePrimitiveType t )
	{
		D3DPRIMITIVETYPE d;
		switch(t)
		{
		case ePrimitiveType_PointList:
			{
				d = D3DPT_POINTLIST;
			}break;

		case ePrimitiveType_LineList:
			{
				d = D3DPT_LINELIST;
			}break;

		case ePrimitiveType_LineStrip:
			{
				d = D3DPT_LINESTRIP;
			}break;

		case ePrimitiveType_TriangleList:
			{
				d = D3DPT_TRIANGLELIST;
			}break;

		case ePrimitiveType_TriangleStrip:
			{
				d = D3DPT_TRIANGLESTRIP;
			}break;

		case ePrimitiveType_TriangleFan:
			{
				d = D3DPT_TRIANGLEFAN;
			}break;
		}

		return d;
	}

}
