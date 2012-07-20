//==========================================================================
/**
* @file	  : Basic.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   11:52
* purpose : 
*/
//==========================================================================

#ifndef __Basic_h__
#define __Basic_h__

//
#include "Common.h"
#include "Radian.h"
#include "Vector3.h"
#include "Ray.h"
//
namespace Euler
{
	bool _MATH_EXPORT_ isInsideTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p);
	  /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the caller may already have it), doesn't need
                normalised since we don't care.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        std::pair<bool, Real> intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c, const Vector3& normal,
            bool positiveSide = true, bool negativeSide = true);

		 /** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        std::pair<bool, Real> _MATH_EXPORT_ intersects(const Ray& ray, const Vector3& a,
            const Vector3& b, const Vector3& c,
            bool positiveSide = true, bool negativeSide = true);
		/** Calculate a face normal without normalize, no w-information. */
		Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

	class _MATH_EXPORT_ Basic
	{
	public:
		Basic();
		~Basic();

	public:
		//
		static inline Real Sqrt (Real fValue) 
		{ 
			return Real(sqrt(fValue)); 
		}

		//
		static inline Real Sqr (Real fValue) 
		{ 
			return fValue*fValue; 
		}

		//
		static inline Real Sin ( const Radian& rValue )
		{
			return sin(rValue._radian);
		}

		//
		static inline Real Cos ( const Radian& rValue )
		{
			return cos(rValue._radian);
		}

		static inline Real Tan (const Radian& rValue ) 
		{
			return tan(rValue._radian); 
		}
		//
		static inline Real Max(const Real a, const Real b)
		{
			return (a > b ? a : b);
		}

		//
		static inline Real Min(const Real a, const Real b)
		{
			return (a < b ? a : b);
		}

		//
		static inline Real randomReal(const Real left, const Real right, const unsigned int random_max = 100)
		{;
			/* initialize random seed: */
			//srand ( time(NULL) );

			/* generate secret number: */
			/*int iSecret = rand() % random_max;

			float result = Min(left, right) + (Max(left, right) - Min(left, right)) * iSecret / random_max;

			//
			return result;
			*/

			float r = (float)(rand() % random_max);
			r /= random_max;

			return left*r + (1-r)*right;
		}

		//
		static inline Vector3 randomVec3Between2Vec3(const Vector3& a, const Vector3& b, const int random_max = 100)
		{
			return Vector3(randomReal(a.x, b.x), randomReal(a.y, b.y), randomReal(a.z, b.z));
		}

		static inline Radian ATan2 (Real fY, Real fX) 
		{
			return Radian(atan2(fY,fX)); 
		}

		static inline Radian ACos (Real f) { return Radian(acos(f)); }

		static inline Real Abs (Real fValue) { return Real(fabs(fValue)); }

		inline static Real power(Real b, Real e)
		{
			return pow(b, e);
		}
	};
}

#endif // __Basic_h__
