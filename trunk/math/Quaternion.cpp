#include "Quaternion.h" 
#include "Matrix3.h"
//
namespace Euler
{
	const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);


	const Quaternion Quaternion::IDENTITY(0.0, 0.0, 0.0, 1.0);


	void Quaternion::ToRotationMatrix( Matrix3& kRot ) const
	{
		Real fTx  = _x+_x;
		Real fTy  = _y+_y;
		Real fTz  = _z+_z;
		Real fTwx = fTx*_w;
		Real fTwy = fTy*_w;
		Real fTwz = fTz*_w;
		Real fTxx = fTx*_x;
		Real fTxy = fTy*_x;
		Real fTxz = fTz*_x;
		Real fTyy = fTy*_y;
		Real fTyz = fTz*_y;
		Real fTzz = fTz*_z;

		kRot[0][0] = 1.0f-(fTyy+fTzz);
		kRot[0][1] = fTxy-fTwz;
		kRot[0][2] = fTxz+fTwy;
		kRot[1][0] = fTxy+fTwz;
		kRot[1][1] = 1.0f-(fTxx+fTzz);
		kRot[1][2] = fTyz-fTwx;
		kRot[2][0] = fTxz-fTwy;
		kRot[2][1] = fTyz+fTwx;
		kRot[2][2] = 1.0f-(fTxx+fTyy);
	}

	Quaternion Quaternion::Slerp( Real fT, const Quaternion& rkP, const Quaternion& rkQ, bool shortestPath /*= false*/ )
	{
		Real fCos = rkP.Dot(rkQ);
		Quaternion rkT;

		// Do we need to invert rotation?
		if (fCos < 0.0f && shortestPath)
		{
			fCos = -fCos;
			rkT = -rkQ;
		}
		else
		{
			rkT = rkQ;
		}

		if (Basic::Abs(fCos) < 1 - ms_fEpsilon)
		{
			// Standard case (slerp)
			Real fSin = Basic::Sqrt(1 - Basic::Sqr(fCos));
			Radian fAngle = Basic::ATan2(fSin, fCos);
			Real fInvSin = 1.0f / fSin;
			Real fCoeff0 = Basic::Sin((1.0f - fT) * fAngle) * fInvSin;
			Real fCoeff1 = Basic::Sin(fT * fAngle) * fInvSin;
			return fCoeff0 * rkP + fCoeff1 * rkT;
		}
		else
		{
			// There are two situations:
			// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
			//    interpolation safely.
			// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
			//    are an infinite number of possibilities interpolation. but we haven't
			//    have method to fix this case, so just use linear interpolation here.
			Quaternion t = (1.0f - fT) * rkP + fT * rkT;
			// taking the complement requires renormalisation
			t.normalize();
			return t;
		}
	}

	Real Quaternion::Dot( const Quaternion& rkQ ) const
	{
		  return _w * rkQ._w + _x * rkQ._x + _y * rkQ._y + _z * rkQ._z;
	}

	//-----------------------------------------------------------------------
	void Quaternion::FromAxes (const Vector3* akAxis)
	{
		Matrix3 kRot;

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			kRot[0][iCol] = akAxis[iCol].x;
			kRot[1][iCol] = akAxis[iCol].y;
			kRot[2][iCol] = akAxis[iCol].z;
		}

		FromRotationMatrix(kRot);
	}
	//-----------------------------------------------------------------------
	void Quaternion::FromAxes (const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
	{
		Matrix3 kRot;

		kRot[0][0] = xaxis.x;
		kRot[1][0] = xaxis.y;
		kRot[2][0] = xaxis.z;

		kRot[0][1] = yaxis.x;
		kRot[1][1] = yaxis.y;
		kRot[2][1] = yaxis.z;

		kRot[0][2] = zaxis.x;
		kRot[1][2] = zaxis.y;
		kRot[2][2] = zaxis.z;

		FromRotationMatrix(kRot);

	}
	//-----------------------------------------------------------------------
	void Quaternion::ToAxes (Vector3* akAxis) const
	{
		Matrix3 kRot;

		ToRotationMatrix(kRot);

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			akAxis[iCol].x = kRot[0][iCol];
			akAxis[iCol].y = kRot[1][iCol];
			akAxis[iCol].z = kRot[2][iCol];
		}
	}

	//-----------------------------------------------------------------------
	void Quaternion::ToAxes (Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const
	{
		Matrix3 kRot;

		ToRotationMatrix(kRot);

		xaxis.x = kRot[0][0];
		xaxis.y = kRot[1][0];
		xaxis.z = kRot[2][0];

		yaxis.x = kRot[0][1];
		yaxis.y = kRot[1][1];
		yaxis.z = kRot[2][1];

		zaxis.x = kRot[0][2];
		zaxis.y = kRot[1][2];
		zaxis.z = kRot[2][2];
	}

	Radian Quaternion::getRoll( bool reprojectAxis /*= true*/ ) const
	{
		if (reprojectAxis)
		{
			// roll = atan2(localx.y, localx.x)
			// pick parts of xAxis() implementation that we need
			//			Real fTx  = 2.0*x;
			Real fTy  = 2.0f*_y;
			Real fTz  = 2.0f*_z;
			Real fTwz = fTz*_w;
			Real fTxy = fTy*_x;
			Real fTyy = fTy*_y;
			Real fTzz = fTz*_z;

			// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

			return Radian(Euler::Basic::ATan2(fTxy+fTwz, 1.0f-(fTyy+fTzz)));

		}
		else
		{
			return Radian(Euler::Basic::ATan2(2*(_x*_y + _w*_z), _w*_w + _x*_x - _y*_y - _z*_z));
		}
	}

	const Real Quaternion::ms_fEpsilon = 1e-03;;

}