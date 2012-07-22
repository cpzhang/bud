//==========================================================================
/**
* @file	  : Common.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   10:45
* purpose : 
*/
//==========================================================================

#ifndef __MathCommon_h__
#define __MathCommon_h__

//
#if defined(math_EXPORTS) // inside DLL
#   define _MATH_EXPORT_   __declspec(dllexport)
#else // outside DLL
#   define _MATH_EXPORT_   __declspec(dllimport)
#endif

//
#include "base/Buddha.h"

//
namespace Zen
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
typedef Zen::Vector2 Vec2;
typedef Zen::Vector3 Vec3;
typedef Zen::Vector4 Vec4;
typedef Zen::Radian  Radian;
typedef Zen::Matrix4 Mat4;
typedef Zen::Matrix3 Mat3;
typedef Zen::AABB AABB;
typedef Zen::Sphere Sphere;
typedef Zen::Quaternion Quaternion;
typedef Zen::Ray Ray;

//
#endif // __MathCommon_h__
 
