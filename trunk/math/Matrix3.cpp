#include "Matrix3.h"

//
namespace Euler
{
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::Transpose () const
	{
		Matrix3 kTranspose;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kTranspose[iRow][iCol] = m[iCol][iRow];
		}
		return kTranspose;
	}
	//-----------------------------------------------------------------------
	Matrix3 Matrix3::operator- () const
	{
		Matrix3 kNeg;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
				kNeg[iRow][iCol] = -m[iRow][iCol];
		}
		return kNeg;
	}
}
