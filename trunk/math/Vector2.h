//==========================================================================
/**
* @file	  : Vector2.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   10:48
* purpose : 
*/
//==========================================================================

#ifndef __Vector2_h__
#define __Vector2_h__
//
#include "Common.h"
//
namespace Euler
{
	class _MATH_EXPORT_ Vector2
	{
	public:
        Real x;
		Real y;

		//
	public:
		inline Vector2()
		{
		}

		inline Vector2(const Real fX, const Real fY )
			: x( fX ), y( fY )
		{
		}

		//
	public:
		 /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline Vector2& operator = ( const Vector2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }
		inline Vector2& operator = ( const Real fScalar)
		{
			x = fScalar;
			y = fScalar;

			return *this;
		}

		//
	public:
		inline bool operator == ( const Vector2& rkVector ) const
		{
			return ( x == rkVector.x && y == rkVector.y );
		}

		inline bool operator != ( const Vector2& rkVector ) const
		{
			return ( x != rkVector.x || y != rkVector.y  );
		}

		// arithmetic operations
		inline Vector2 operator + ( const Vector2& rkVector ) const
		{
			return Vector2(
				x + rkVector.x,
				y + rkVector.y);
		}

		inline Vector2 operator - ( const Vector2& rkVector ) const
		{
			return Vector2(
				x - rkVector.x,
				y - rkVector.y);
		}

		inline Vector2 operator * ( const Real fScalar ) const
		{
			return Vector2(
				x * fScalar,
				y * fScalar);
		}

		inline friend Vector2 operator * ( const Real fScalar, const Vector2& rkVector )
		{
			return Vector2(
				fScalar * rkVector.x,
				fScalar * rkVector.y);
		}

		inline Vector2 operator * ( const Vector2& rhs) const
		{
			return Vector2(
				x * rhs.x,
				y * rhs.y);
		}

		inline Vector2 operator / ( const Real fScalar ) const
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			return Vector2(
				x * fInv,
				y * fInv);
		}

		inline Vector2 operator / ( const Vector2& rhs) const
		{
			return Vector2(
				x / rhs.x,
				y / rhs.y);
		}

		inline const Vector2& operator + () const
		{
			return *this;
		}

		inline Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

		// arithmetic updates
		inline Vector2& operator += ( const Vector2& rkVector )
		{
			x += rkVector.x;
			y += rkVector.y;

			return *this;
		}
		inline Vector2& operator -= ( const Vector2& rkVector )
		{
			x -= rkVector.x;
			y -= rkVector.y;

			return *this;
		}

		inline Vector2& operator *= ( const Vector2& rkVector )
		{
			x *= rkVector.x;
			y *= rkVector.y;

			return *this;
		}

		inline Vector2& operator /= ( const Real fScalar )
		{
			assert( fScalar != 0.0 );

			Real fInv = 1.0f / fScalar;

			x *= fInv;
			y *= fInv;

			return *this;
		}

		//
		public:
			// special points
			static const Vector2 ZERO;
			static const Vector2 UNIT_X;
			static const Vector2 UNIT_Y;
			static const Vector2 NEGATIVE_UNIT_X;
			static const Vector2 NEGATIVE_UNIT_Y;
			static const Vector2 UNIT_SCALE;
	};
}

#endif // __Vector2_h__
 
