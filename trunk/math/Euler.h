//==========================================================================
/**
* @file	  : Euler.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-23   18:57
* purpose : 
*/
//==========================================================================

#ifndef __Euler_h__
#define __Euler_h__

// This file includes all the other files which you will need to build a client application
//#include "Common.h"
//
#include "Radian.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Quaternion.h"
#include "AABB.h"
#include "Sphere.h"
#include "Ray.h"
#include "Complex.h"
#include "RootFinding.h"
namespace Euler
{
	static const Real PI(((FLOAT)  3.141592654f));

	static const Real TwoPI(PI * 2.0f);

	static const Real HalfPI(PI * 0.5f);
}
#endif // __Euler_h__
