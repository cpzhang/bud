//==========================================================================
/**
* @file	  : Matrix3.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   17:42
* purpose : 
*/
//==========================================================================

#ifndef __Matrix3_h__
#define __Matrix3_h__

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

#include "Common.h"
#include "Vector3.h"

//
namespace Euler
{
	class _MATH_EXPORT_ Matrix3
	{
	public:
		friend class Matrix4;
		Real m[3][3];

		//
	public:
		inline Matrix3 () {};

		inline Matrix3 (Real fEntry00, Real fEntry01, Real fEntry02,
			Real fEntry10, Real fEntry11, Real fEntry12,
			Real fEntry20, Real fEntry21, Real fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

		//
	public:
		// assignment and comparison
		inline Matrix3& operator= (const Matrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(Real));
			return *this;
		}

		// member access, allows use of construct mat[r][c]
		inline Real* operator[] (size_t iRow) const
		{
			return (Real*)m[iRow];
		}

		// matrix * vector [3x3 * 3x1 = 3x1]
		inline Vector3 operator* (const Vector3& rkPoint) const
		{
			Vector3 kProd;
			for (size_t iRow = 0; iRow < 3; iRow++)
			{
				kProd[iRow]  =	m[iRow][0]*rkPoint[0] +
								m[iRow][1]*rkPoint[1] +
								m[iRow][2]*rkPoint[2];
			}
			return kProd;
		}
		Matrix3 Transpose () const;
		Matrix3 operator- () const;
	};
}

#endif // __Matrix3_h__
