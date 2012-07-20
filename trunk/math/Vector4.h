//==========================================================================
/**
* @file	  : Vector4.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   12:32
* purpose : 
*/
//==========================================================================

#ifndef __Vector4_h__
#define __Vector4_h__

//
#include "Common.h"

//
namespace Euler
{
	class _MATH_EXPORT_ Vector4
	{
	public:
		Real x;
		Real y;
		Real z;
		Real w;
	public:
		//
		inline Vector4()
			:	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}
		//
		inline Vector4(const Vector4& rkVector)
			: x(rkVector.x), y(rkVector.y), z(rkVector.z), w(rkVector.w)
		{

		}
		//
		inline Vector4( const Real fX, const Real fY, const Real fZ, const Real fW )
			: x( fX ), y( fY ), z( fZ ), w( fW )
		{
		}

		//
		inline Vector4(Real v)
			:x(v), y(v), z(v), w(v)
		{

		}
	public:
		
		inline Real operator [] ( const size_t i ) const
		{
			assert( i < 4 );

			return *(&x+i);
		}

		inline Real& operator [] ( const size_t i )
		{
			assert( i < 4 );

			return *(&x+i);
		}

		//
		inline bool operator == ( const Vector4& rkVector ) const
		{
			return ( x == rkVector.x && y == rkVector.y && z == rkVector.z && w == rkVector.w );
		}
		//
		inline bool operator != ( const Vector4& rkVector ) const
		{
			return ( x != rkVector.x ||	y != rkVector.y ||	z != rkVector.z ||	w != rkVector.w );
		}
		//
		inline Vector4& operator = ( const Vector4& rkVector )
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;
			w = rkVector.w;

			return *this;
		}
		// arithmetic operations
		inline Vector4 operator + ( const Vector4& rkVector ) const
		{
			return Vector4(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z,
				w + rkVector.w);
		}

		inline Vector4 operator - ( const Vector4& rkVector ) const
		{
			return Vector4(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z,
				w - rkVector.w);
		}

		inline Vector4 operator * ( const Real fScalar ) const
		{
			return Vector4(
				x * fScalar,
				y * fScalar,
				z * fScalar,
				w * fScalar);
		}

		inline Vector4 operator * ( const Vector4& rhs) const
		{
			return Vector4(
				rhs.x * x,
				rhs.y * y,
				rhs.z * z,
				rhs.w * w);
		}

		inline Vector4 operator / ( const Real fScalar ) const
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			return Vector4(
				x * fInv,
				y * fInv,
				z * fInv,
				w * fInv);
		}

		inline Vector4 operator / ( const Vector4& rhs) const
		{
			return Vector4(
				x / rhs.x,
				y / rhs.y,
				z / rhs.z,
				w / rhs.w);
		}

		inline const Vector4& operator + () const
		{
			return *this;
		}

		inline Vector4 operator - () const
		{
			return Vector4(-x, -y, -z, -w);
		}
		// arithmetic updates
		inline Vector4& operator += ( const Vector4& rkVector )
		{
			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;
			w += rkVector.w;

			return *this;
		}

		inline Vector4& operator -= ( const Vector4& rkVector )
		{
			x -= rkVector.x;
			y -= rkVector.y;
			z -= rkVector.z;
			w -= rkVector.w;

			return *this;
		}

		inline Vector4& operator *= ( const Real fScalar )
		{
			x *= fScalar;
			y *= fScalar;
			z *= fScalar;
			w *= fScalar;
			return *this;
		}

		inline Vector4& operator += ( const Real fScalar )
		{
			x += fScalar;
			y += fScalar;
			z += fScalar;
			w += fScalar;
			return *this;
		}

		inline Vector4& operator -= ( const Real fScalar )
		{
			x -= fScalar;
			y -= fScalar;
			z -= fScalar;
			w -= fScalar;
			return *this;
		}

		inline Vector4& operator *= ( const Vector4& rkVector )
		{
			x *= rkVector.x;
			y *= rkVector.y;
			z *= rkVector.z;
			w *= rkVector.w;

			return *this;
		}

		inline Vector4& operator /= ( const Real fScalar )
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;
			z *= fInv;
			w *= fInv;

			return *this;
		}

		inline Vector4& operator /= ( const Vector4& rkVector )
		{
			x /= rkVector.x;
			y /= rkVector.y;
			z /= rkVector.z;
			w /= rkVector.w;

			return *this;
		}
	};
}

#endif // __Vector4_h__
  
