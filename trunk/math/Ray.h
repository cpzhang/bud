//==========================================================================
/**
* @file	  : Ray.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-5   17:53
* purpose : 
*/
//==========================================================================

#ifndef __Ray_h__
#define __Ray_h__

#include "Common.h"
#include "Vector3.h"

namespace Zen
{
	class _MATH_EXPORT_ Ray
	{
	public:
		Vector3	_direction;
		Vector3	_origin;

	public:
		Ray();
		Ray(const Vector3& dir, const Vector3& origin);

	public:
		Vector3 getPoint(Real t) const;

		/** Gets the origin of the ray. */
		const Vector3& getOrigin(void) const {return _origin;} 

		/** Gets the direction of the ray. */
		const Vector3& getDirection(void) const {return _direction;} 
	};
}

#endif // __Ray_h__
 
