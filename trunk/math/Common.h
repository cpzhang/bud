//==========================================================================
/**
* @file	  : Common.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   10:45
* purpose : 
*/
//==========================================================================

#ifndef __EulerCommon_h__
#define __EulerCommon_h__

//
#if defined(math_EXPORTS) // inside DLL
#   define _MATH_EXPORT_   __declspec(dllexport)
#else // outside DLL
#   define _MATH_EXPORT_   __declspec(dllimport)
#endif

//
#include "base/Buddha.h"

//
namespace Euler
{
	//
	class Basic;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Quaternion;
	class Radian;
	class Ray;
	class AABB;
	class Sphere;
	class Complex;
	class RootFinding;
	class Polynomial;
}

//
typedef Euler::Vector2 Vec2;
typedef Euler::Vector3 Vec3;
typedef Euler::Vector4 Vec4;
typedef Euler::Radian  Radian;
typedef Euler::Matrix4 Mat4;
typedef Euler::Matrix3 Mat3;
typedef Euler::AABB AABB;
typedef Euler::Sphere Sphere;
typedef Euler::Quaternion Quaternion;
typedef Euler::Ray Ray;

//
#endif // __EulerCommon_h__
 
