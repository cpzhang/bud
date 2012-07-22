//==========================================================================
/**
* @file	  : Quaternion.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-6   11:36
* purpose : 
*/
//==========================================================================

#ifndef __Quaternion_h__
#define __Quaternion_h__

//
#include "Common.h"
#include "Basic.h"
#include "Vector3.h"
#include "Radian.h"
#include "Matrix3.h"
//
namespace Zen
{
	class _MATH_EXPORT_ Quaternion
	{
		//
	public:
		Real	_x;
		Real	_y;
		Real	_z;
		Real	_w;
	public:
		inline Quaternion(Real x = 0.0, Real y = 0.0, Real z = 0.0, Real w = 1.0)
			:	_x(x), _y(y), _z(z), _w(w)
		{

		}
		/// Construct a quaternion from a rotation matrix
		inline Quaternion(const Matrix3& rot)
		{
			this->FromRotationMatrix(rot);
		}
		inline Quaternion(const Quaternion& q)
		{
			_x = q._x;
			_y = q._y;
			_z = q._z;
			_w = q._w;
		}
		/// Construct a quaternion from an angle/axis
		inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
		{
			this->FromAngleAxis(rfAngle, rkAxis);
		}
		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
		{
			this->FromAxes(xaxis, yaxis, zaxis);
		}
		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3* akAxis)
		{
			this->FromAxes(akAxis);
		}
		/// Construct a quaternion from 4 manual w/x/y/z values
		inline Quaternion(Real* valptr)
		{
			memcpy(&_x, valptr, sizeof(Real)*4);
		}

		~Quaternion(){}

		//
	public:
		inline Quaternion& operator = (const Quaternion& q)
		{
			_x = q._x;
			_y = q._y;
			_z = q._z;
			_w = q._w;

			//
			return *this;
		}

		//
		inline bool operator == (const Quaternion& q) const
		{
			return (_x == q._x) && (_y == q._y) && (_z == q._z) && (_w == q._w);
		}

		//
		inline bool operator != (const Quaternion& q) const
		{
			return !(operator==(q));
		}

		//
		inline Quaternion operator + (const Quaternion& q) const
		{
			return Quaternion(_x + q._x, _y + q._y, _z + q._z, _w + q._w);
		}

		//
		inline Quaternion operator - (const Quaternion& q) const
		{
			return Quaternion(_x - q._x, _y - q._y, _z - q._z, _w - q._w);
		}

		//
		inline Quaternion operator * (const Quaternion& q) const
		{
			return Quaternion(
					 _x * q._w + _y * q._z - _z * q._y + _w * q._x,
					-_x * q._z + _y * q._w + _z * q._x + _w * q._y,
					 _x * q._y - _y * q._x + _z * q._w + _w * q._z,
					-_x * q._x - _y * q._y - _z * q._z + _w * q._w
				);
		}

		//
		inline Quaternion operator * (const Real scale) const
		{
			return Quaternion(_x * scale, _y * scale, _z * scale, _w * scale);
		}

		//
		inline friend Quaternion operator* (Real scale, const Quaternion& q)
		{
			return q * scale;
		}
		
		//
		inline Quaternion operator - () const
		{
			return Quaternion(-_x, -_y, -_z, -_w);
		}

		// rotation of a vector by a quaternion
		inline Vector3 operator* (const Vector3& v) const
		{
			// nVidia SDK implementation
			Vector3 uv, uuv;
			Vector3 qvec(_x, _y, _z);
			uv = qvec.crossProduct(v);
			uuv = qvec.crossProduct(uv);
			uv *= (2.0f * _w);
			uuv *= 2.0f;

			return v + uv + uuv;
		}

		//
	public:
		//
		inline Quaternion conjugate() const
		{
			return Quaternion(-_x, -_y, -_z, _w);
		}

		//
		inline Real normSquare() const
		{
			return _x * _x + _y * _y + _z * _z + _w * _w;
		}

		//
		inline Real norm() const
		{
			return Basic::Sqrt(normSquare());
		}

		//
		inline Real normalize() 
		{
			Real len = norm();
			if (len > 0.0)
			{
				Real inv = 1.0f / len;
				*this = *this * inv;
			}

			//
			return len;
		}

		//
		inline Quaternion inverse() const
		{
			Real fNormSqu = normSquare();
			if (fNormSqu > 0.0)
			{
				Real fInv = 1.0f / fNormSqu;
				return conjugate() * fInv;
			}

			//
			return ZERO;
		}

		//
		inline void FromAngleAxis (const Radian& rfAngle,	const Vector3& rkAxis)
		{
			// assert:  axis[] is unit length
			//
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

			Radian fHalfAngle ( 0.5*rfAngle );
			Real fSin = Basic::Sin(fHalfAngle);

			_x = fSin*rkAxis.x;
			_y = fSin*rkAxis.y;
			_z = fSin*rkAxis.z;
			_w = Basic::Cos(fHalfAngle);
		}

		void ToAngleAxis (Radian& rfAngle, Vector3& rkAxis) const
		{
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

			Real fSqrLength = _x * _x + _y * _y + _z * _z;
			if ( fSqrLength > 0.0 )
			{
				rfAngle = 2.0 * Basic::ACos(_w);
				Real fInvLength = 1.0f / Basic::Sqrt(fSqrLength);
				rkAxis.x = _x*fInvLength;
				rkAxis.y = _y*fInvLength;
				rkAxis.z = _z*fInvLength;
			}
			else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				rfAngle = Radian(0.0);
				rkAxis.x = 1.0;
				rkAxis.y = 0.0;
				rkAxis.z = 0.0;
			}
		}

		//-----------------------------------------------------------------------
		void Quaternion::FromRotationMatrix (const Matrix3& kRot)
		{
			// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
			// article "Quaternion Calculus and Fast Animation".
			Real fTrace = kRot[0][0] + kRot[1][1] + kRot[2][2];
			Real fRoot;

			if ( fTrace > 0.0 )
			{
				// |w| > 1/2, may as well choose w > 1/2
				fRoot = Basic::Sqrt(fTrace + 1.0f);  // 2w
				_w = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;  // 1/(4w)
				_x = (kRot[2][1]-kRot[1][2])*fRoot;
				_y = (kRot[0][2]-kRot[2][0])*fRoot;
				_z = (kRot[1][0]-kRot[0][1])*fRoot;
			}
			else
			{
				// |w| <= 1/2
				static size_t s_iNext[3] = { 1, 2, 0 };
				size_t i = 0;
				if ( kRot[1][1] > kRot[0][0] )
					i = 1;
				if ( kRot[2][2] > kRot[i][i] )
					i = 2;
				size_t j = s_iNext[i];
				size_t k = s_iNext[j];

				fRoot = Basic::Sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
				Real* apkQuat[3] = { &_x, &_y, &_z };
				*apkQuat[i] = 0.5f*fRoot;
				fRoot = 0.5f/fRoot;
				_w = (kRot[k][j]-kRot[j][k])*fRoot;
				*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
				*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
			}
		}

		 void ToRotationMatrix (Matrix3& kRot) const;

		 // functions of a quaternion
		 Real Dot (const Quaternion& rkQ) const;  // dot product

		/** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
			@See FromAxes
		*/
        void FromAxes (const Vector3* akAxis);
        void FromAxes (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		/** Gets the 3 orthonormal axes defining the quaternion. @See FromAxes */
        void ToAxes (Vector3* akAxis) const;
        void ToAxes (Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;
		Radian getRoll(bool reprojectAxis = true) const;
	public:
		 // spherical linear interpolation
		 static Quaternion Slerp (Real fT, const Quaternion& rkP, const Quaternion& rkQ, bool shortestPath = false);
		//
	public:
		// special values
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

		// cutoff for sine near zero
		static const Real ms_fEpsilon;
	};
}

#endif // __Quaternion_h__
