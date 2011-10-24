//==========================================================================
/**
* @file	  : AABB.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-23   16:31
* purpose : 
*/
//==========================================================================

#ifndef __AABB_h__
#define __AABB_h__

#include "Common.h"
#include "Vector3.h"

namespace Euler
{
	 /** A 3D box aligned with the x/y/z axes.
	    @remarks
		    This class represents a simple box which is aligned with the
		    axes. Internally it only stores 2 points as the extremeties of
		    the box, one which is the minima of all 3 axes, and the other
		    which is the maxima of all 3 axes. This class is typically used
		    for an axis-aligned bounding box (AABB) for collision and
		    visibility determination.
    */

	class _MATH_EXPORT_ AABB
	{
	public:
		Vec3	_min;
		Vec3	_max;

	public:
		AABB()
			: _min(Vec3::ZERO), _max(Vec3::ZERO)
		{

		}
		AABB(const Vec3& min, const Vec3& max)
			:	_min(min), _max(max)
		{

		}

	public:
		inline void updateMin(const Vec3& p)
		{
			if (p.x < _min.x)
			{
				_min.x = p.x;
			}

			if (p.y < _min.y)
			{
				_min.y = p.y;
			}

			if (p.z < _min.z)
			{
				_min.z = p.z;
			}
		}

		inline void updateMax(const Vec3& p)
		{
			if (p.x > _max.x)
			{
				_max.x = p.x;
			}

			if (p.y > _max.y)
			{
				_max.y = p.y;
			}

			if (p.z > _max.z)
			{
				_max.z = p.z;
			}
		}
	};
}

#endif // __AABB_h__
 
