#include "Basic.h" 

//
namespace Euler
{
	Basic::Basic()
	{

	}

	Basic::~Basic()
	{

	}
	//-----------------------------------------------------------------------
	Vector3 Basic::calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
		return normal;
	}

	//-----------------------------------------------------------------------
	std::pair<bool, Real> Basic::intersects(const Ray& ray, const Vector3& a,
		const Vector3& b, const Vector3& c,
		bool positiveSide, bool negativeSide)
	{
		Vector3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
		return intersects(ray, a, b, c, normal, positiveSide, negativeSide);
	}
	//-----------------------------------------------------------------------
	std::pair<bool, Real> Basic::intersects(const Ray& ray, const Vector3& a,
		const Vector3& b, const Vector3& c, const Vector3& normal,
		bool positiveSide, bool negativeSide)
	{
		//
		// Calculate intersection with plane.
		//
		Real t;
		{
			Real denom = normal.dotProduct(ray.getDirection());

			// Check intersect side
			if (denom > + std::numeric_limits<Real>::epsilon())
			{
				if (!negativeSide)
					return std::pair<bool, Real>(false, 0);
			}
			else if (denom < - std::numeric_limits<Real>::epsilon())
			{
				if (!positiveSide)
					return std::pair<bool, Real>(false, 0);
			}
			else
			{
				// Parallel or triangle area is close to zero when
				// the plane normal not normalised.
				return std::pair<bool, Real>(false, 0);
			}

			t = normal.dotProduct(a - ray.getOrigin()) / denom;

			if (t < 0)
			{
				// Intersection is behind origin
				return std::pair<bool, Real>(false, 0);
			}
		}

		//
		// Calculate the largest area projection plane in X, Y or Z.
		//
		size_t i0, i1;
		{
			Real n0 = Basic::Abs(normal[0]);
			Real n1 = Basic::Abs(normal[1]);
			Real n2 = Basic::Abs(normal[2]);

			i0 = 1; i1 = 2;
			if (n1 > n2)
			{
				if (n1 > n0) i0 = 0;
			}
			else
			{
				if (n2 > n0) i1 = 0;
			}
		}

		//
		// Check the intersection point is inside the triangle.
		//
		{
			Real u1 = b[i0] - a[i0];
			Real v1 = b[i1] - a[i1];
			Real u2 = c[i0] - a[i0];
			Real v2 = c[i1] - a[i1];
			Real u0 = t * ray.getDirection()[i0] + ray.getOrigin()[i0] - a[i0];
			Real v0 = t * ray.getDirection()[i1] + ray.getOrigin()[i1] - a[i1];

			Real alpha = u0 * v2 - u2 * v0;
			Real beta  = u1 * v0 - u0 * v1;
			Real area  = u1 * v2 - u2 * v1;

			// epsilon to avoid float precision error
			const Real EPSILON = 1e-6f;

			Real tolerance = - EPSILON * area;

			if (area > 0)
			{
				if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
					return std::pair<bool, Real>(false, 0);
			}
			else
			{
				if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
					return std::pair<bool, Real>(false, 0);
			}
		}

		return std::pair<bool, Real>(true, t);
	}
}