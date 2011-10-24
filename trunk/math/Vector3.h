//==========================================================================
/**
* @file	  : Vector3.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   12:32
* purpose : 
*/
//==========================================================================

#ifndef __Vector3_h__
#define __Vector3_h__

//
#include "Common.h"
#include "Vector4.h"

//
namespace Euler
{
	class _MATH_EXPORT_ Vector3
	{
	public:
		Real x;
		Real y;
		Real z;
	public:
		//
		inline Vector3()
			:	x(0.0f), y(0.0f), z(0.0f)
		{
		}
		//
		inline Vector3(const Vector3& rkVector)
			: x(rkVector.x), y(rkVector.y), z(rkVector.z)
		{

		}

		//
		inline Vector3(const Vector4& rkVector)
			: x(rkVector.x), y(rkVector.y), z(rkVector.z)
		{

		}

		//
		inline Vector3( const Real fX, const Real fY, const Real fZ )
			: x( fX ), y( fY ), z( fZ )
		{
		}

		//
		inline Vector3( const Real fX)
			: x( fX ), y( fX ), z( fX )
		{
		}

	public:
		//
		inline Real operator [] ( const size_t i ) const
		{
			assert( i < 3 );

			return *(&x+i);
		}

		//
		inline Real& operator [] ( const size_t i )
		{
			assert( i < 3 );

			return *(&x+i);
		}

		//
		inline bool operator == ( const Vector3& rkVector ) const
		{
			return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
		}
		//
		inline bool operator != ( const Vector3& rkVector ) const
		{
			return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
		}
		//
		inline Vector3& operator = ( const Vector3& rkVector )
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;

			return *this;
		}
		// arithmetic operations
		inline Vector3 operator + ( const Vector3& rkVector ) const
		{
			return Vector3(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z);
		}

		inline Vector3 operator - ( const Vector3& rkVector ) const
		{
			return Vector3(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z);
		}

		inline Vector3 operator * ( const Real fScalar ) const
		{
			return Vector3(
				x * fScalar,
				y * fScalar,
				z * fScalar);
		}

		inline friend Vector3 operator * ( const Real fScalar, const Vector3& rkVector )
		{
			return Vector3(
				fScalar * rkVector.x,
				fScalar * rkVector.y,
				fScalar * rkVector.z);
		}

		inline Vector3 operator * ( const Vector3& rhs) const
		{
			return Vector3(
				x * rhs.x,
				y * rhs.y,
				z * rhs.z);
		}

		inline Vector3 operator / ( const Real fScalar ) const
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			return Vector3(
				x * fInv,
				y * fInv,
				z * fInv);
		}

		inline Vector3 operator / ( const Vector3& rhs) const
		{
			return Vector3(
				x / rhs.x,
				y / rhs.y,
				z / rhs.z);
		}

		inline const Vector3& operator + () const
		{
			return *this;
		}

		inline Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}
		// arithmetic updates
		inline Vector3& operator += ( const Vector3& rkVector )
		{
			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;

			return *this;
		}

		inline Vector3& operator += ( const Real fScalar )
		{
			x += fScalar;
			y += fScalar;
			z += fScalar;
			return *this;
		}

		inline Vector3& operator -= ( const Vector3& rkVector )
		{
			x -= rkVector.x;
			y -= rkVector.y;
			z -= rkVector.z;

			return *this;
		}

		inline Vector3& operator -= ( const Real fScalar )
		{
			x -= fScalar;
			y -= fScalar;
			z -= fScalar;
			return *this;
		}

		inline Vector3& operator *= ( const Real fScalar )
		{
			x *= fScalar;
			y *= fScalar;
			z *= fScalar;
			return *this;
		}

		inline Vector3& operator *= ( const Vector3& rkVector )
		{
			x *= rkVector.x;
			y *= rkVector.y;
			z *= rkVector.z;

			return *this;
		}

		inline Vector3& operator /= ( const Real fScalar )
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;

			return *this;
		}

		inline Vector3& operator /= ( const Vector3& rkVector )
		{
			x /= rkVector.x;
			y /= rkVector.y;
			z /= rkVector.z;

			return *this;
		}
	public:
		//
		Real length () const;

		//
		Real normalise();

		//
		inline Real dotProduct(const Vector3& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		//
		inline Vector3 crossProduct( const Vector3& rkVector ) const
		{
			return Vector3(
				y * rkVector.z - z * rkVector.y,
				z * rkVector.x - x * rkVector.z,
				x * rkVector.y - y * rkVector.x);
		}
	public:
		// special points
		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 UNIT_SCALE;
	};
}

#endif // __Vector3_h__
 
