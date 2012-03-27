#include "Color.h"

namespace Euclid
{

	Color::Color( Real red /*= 1.0f*/, Real green /*= 1.0f*/, Real blue /*= 1.0f*/, Real alpha /*= 1.0f*/ )
		: r(red), g(green), b(blue), a(alpha)
	{

	}

	RGBA Color::getRGBA() const
	{
		u8 val;
		RGBA col = 0;

		// red
		val = static_cast<u8>(r*255);
		col += val<<24;


		// green
		val = static_cast<u8>(g*255);
		col += val<<16;


		// blue
		val = static_cast<u8>(b*255);
		col += val<<8;


		// alpha
		val = static_cast<u8>(a*255);
		col += val;

		return col;
	}

	RGBA Color::getARGB() const
	{
		u8 val;
		RGBA col = 0;

		// red
		val = static_cast<u8>(a*255);
		col += val<<24;


		// green
		val = static_cast<u8>(r*255);
		col += val<<16;


		// blue
		val = static_cast<u8>(g*255);
		col += val<<8;


		// alpha
		val = static_cast<u8>(b*255);
		col += val;

		return col;
	}

	void Color::random()
	{
		r = Euler::Basic::randomReal(0.0f, 1.0f);
		g = Euler::Basic::randomReal(0.0f, 1.0f);
		b = Euler::Basic::randomReal(0.0f, 1.0f);
		a = Euler::Basic::randomReal(0.0f, 1.0f);
	}

	void Color::setABGR( u32 val32 )
	{
		// Alpha
		a = static_cast<u8>(val32 >> 24) / 255.0f;

		// Blue
		b = static_cast<u8>(val32 >> 16) / 255.0f;

		// Green
		g = static_cast<u8>(val32 >> 8) / 255.0f;

		// Red
		r = static_cast<u8>(val32) / 255.0f;
	}

	const Color Color::Blue = Color(0.0f, 0.0f, 1.0f); 

	const Color Color::Green = Color(0.0f, 1.0f, 0.0f);

	const Color Color::Red = Color(1.0f, 0.0f, 0.0f);

	const Color Color::White = Color(1.0f, 1.0f, 1.0f);

	const Color Color::Black = Color(0.0f, 0.0f, 0.0f);

	Color3::Color3( Real red /*= 1.0f*/, Real green /*= 1.0f*/, Real blue /*= 1.0f*/)
		: r(red), g(green), b(blue)
	{

	}

	RGBA Color3::getRGBA() const
	{
		u8 val;
		RGBA col = 0;

		// red
		val = static_cast<u8>(r*255);
		col += val<<24;


		// green
		val = static_cast<u8>(g*255);
		col += val<<16;


		// blue
		val = static_cast<u8>(b*255);
		col += val<<8;


		// alpha
		val = static_cast<u8>(1*255);
		col += val;

		return col;
	}

	void Color3::random()
	{
		r = Euler::Basic::randomReal(0.0f, 1.0f);
		g = Euler::Basic::randomReal(0.0f, 1.0f);
		b = Euler::Basic::randomReal(0.0f, 1.0f);
	}

	const Color3 Color3::Blue = Color3(0.0f, 0.0f, 1.0f); 

	const Color3 Color3::Green = Color3(0.0f, 1.0f, 0.0f);

	const Color3 Color3::Red = Color3(1.0f, 0.0f, 0.0f);

	const Color3 Color3::White = Color3(1.0f, 1.0f, 1.0f);

	const Color3 Color3::Black = Color3(0.0f, 0.0f, 0.0f);

}
