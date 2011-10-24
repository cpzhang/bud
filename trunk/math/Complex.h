/********************************************************************
**	file: 		Complex.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-27
**	purpose:	
*********************************************************************/
#ifndef __Complex_h__
#define __Complex_h__

//
#include "Common.h"

//
namespace Euler
{
	class _MATH_EXPORT_ Complex
	{
	public:
		Real realPart, imagePart;

	public:
		Complex()
			:realPart(0.0f), imagePart(0.0f)
		{

		}

		Complex(Real r)
			:realPart(r), imagePart(0.0f)
		{

		}

		Complex(Real r, Real i)
			:realPart(r), imagePart(i)
		{

		}

	public:
		//
		bool isPureReal()
		{
			return imagePart == 0.0f;
		}

		//
		inline bool operator ==(const Complex& c) const
		{
			return realPart == c.realPart && imagePart == c.imagePart;
		}

		//
		inline bool operator !=(const Complex& c) const
		{
			return realPart != c.realPart || imagePart != c.imagePart;
		}

		//
		inline Complex operator +(const Complex& c) const
		{
			return Complex(realPart + c.realPart, imagePart + c.imagePart);
		}
		
		//
		inline Complex operator -(const Complex& c) const
		{
			return Complex(realPart - c.realPart, imagePart - c.imagePart);
		}

		//
		inline Complex& operator +=(const Complex& c)
		{
			realPart += c.realPart;
			imagePart += c.imagePart;

			return *this;
		}

		//
		inline Complex& operator -=(const Complex& c)
		{
			realPart -= c.realPart;
			imagePart -= c.imagePart;

			return *this;
		}
	};
}

#endif // __Complex_h__
