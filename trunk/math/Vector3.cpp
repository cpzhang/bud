#include "Vector3.h"
#include "Basic.h"

//
namespace Zen
{
	const Vector3 Vector3::ZERO( 0, 0, 0 );

	const Vector3 Vector3::UNIT_X( 1, 0, 0 );
	const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
	const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
	const Vector3 Vector3::NEGATIVE_UNIT_X( -1,  0,  0 );
	const Vector3 Vector3::NEGATIVE_UNIT_Y(  0, -1,  0 );
	const Vector3 Vector3::NEGATIVE_UNIT_Z(  0,  0, -1 );
	const Vector3 Vector3::UNIT_SCALE(1, 1, 1);

	Real Vector3::length() const
	{
		return Basic::Sqrt( x * x + y * y + z * z );
	}

	Real Vector3::normalise()
	{
		Real fLength = Basic::Sqrt( x * x + y * y + z * z );

		// Will also work for zero-sized vectors, but will change nothing
		if ( fLength > 1e-08 )
		{
			Real fInvLength = 1.0f / fLength;
			x *= fInvLength;
			y *= fInvLength;
			z *= fInvLength;
		}

		return fLength;
	}
}
