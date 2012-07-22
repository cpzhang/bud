#include "DXMapping.h"

//
namespace Zen
{
	//---------------------------------------------------------------------
	D3DXMATRIX DXMapping::toDXMatrix( const Mat4& mat )
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
	Mat4 DXMapping::fromDXMatrix( const D3DXMATRIX& mat )
	{
		return Mat4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
	}
}
