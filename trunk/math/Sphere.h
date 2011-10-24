//==========================================================================
/**
* @file	  : Sphere.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-23   16:34
* purpose : 
*/
//==========================================================================

#ifndef __Sphere_h__
#define __Sphere_h__

#include "Common.h"
#include "Vector3.h"
#include "AABB.h"

namespace Euler
{
	/** A sphere primitive, mostly used for bounds checking. 
    @remarks
        A sphere in math texts is normally represented by the function
        x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). Ogre stores spheres
        simply as a center point and a radius.
    */

	class _MATH_EXPORT_ Sphere
	{
	public:
		Vec3	_center;
		Real	_radius;
	public:
		Sphere()
			: _center(Vec3::ZERO), _radius(0.0f)
		{

		}

		Sphere(const Vec3& c, const Real r)
			: _center(c), _radius(r)
		{

		}

		// the AABB is fully surrounded by the Sphere
		Sphere(const AABB& aabb)
		{
			_center = (aabb._min + aabb._max) * 0.5f;
			_radius = (aabb._max - aabb._min).length() * 0.5f;
		}

	public:
		inline Sphere& operator = (const Sphere& s)
		{
			_center = s._center;
			_radius = s._radius;

			return *this;
		}

		inline void setCenter(const Vec3& c)
		{
			_center = c;
		}

		inline void setRadius(const Real r)
		{
			_radius = r;
		}
	};
}

#endif // __Sphere_h__
 
