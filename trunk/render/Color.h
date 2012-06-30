//==========================================================================
/**
* @file	  : Color.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-21   10:28
* purpose : 
*/
//==========================================================================

#ifndef __Color_h__
#define __Color_h__

#include "Common.h"

namespace Euclid
{
	typedef u32 RGBA;

	class _EuclidExport_ Color
	{
	public:
		Real r, g, b, a;

	public:
		Color(Real red = 1.0f, Real green = 1.0f, Real blue = 1.0f, Real alpha = 1.0f);

	public:
		RGBA getRGBA() const;
		RGBA getARGB() const;

		void setABGR(u32 i);
		void setRGBA(u32 i);
		void random();

	public:
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	};

	class _EuclidExport_ Color3
	{
	public:
		Real r, g, b;

	public:
		Color3(Real red = 1.0f, Real green = 1.0f, Real blue = 1.0f);

		Color3(Real v)
			:r(v), g(v), b(v)
		{

		}
	public:
		RGBA getRGBA() const;

		void random();

		inline Color3 operator = (Real v)
		{
			r = v;
			g = v;
			b = v;

			return *this;
		}
		inline Color3 operator = (const Color3 v)
		{
			r = v.r;
			g = v.g;
			b = v.b;

			return *this;
		}
		inline Color3 operator * (const Real v) const
		{
			return Color3(r*v, g*v, b*v);
		}
		inline Color3 operator + (const Color3& c)
		{
			return Color3(r + c.r, g + c.g, b + c.b);
		}
		friend Color3 operator * (const Real v, const Color3& c)
		{
			return Color3(c.r*v, c.g*v, c.b*v);
		}
	public:
		static const Color3 Black;
		static const Color3 White;
		static const Color3 Red;
		static const Color3 Green;
		static const Color3 Blue;
	};
}

#endif // __Color_h__
 
