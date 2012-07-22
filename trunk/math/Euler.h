//==========================================================================
/**
* @file	  : Zen.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   18:57
* purpose : 
*/
//==========================================================================

#ifndef __EulerCommon_h__
#define __EulerCommon_h__

// This file includes all the other files which you will need to build a client application
//#include "Common.h"
//
#include "Radian.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "AABB.h"
#include "Sphere.h"
#include "Ray.h"
#include "Complex.h"
#include "RootFinding.h"
namespace Zen
{
	static const Real PI(((FLOAT)  3.141592654f));

	static const Real TwoPI(PI * 2.0f);

	static const Real HalfPI(PI * 0.5f);
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
#endif // __EulerCommon_h__
