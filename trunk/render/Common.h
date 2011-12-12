//==========================================================================
/**
* @file	  : Common.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-24   21:15
* purpose : 
*/
//==========================================================================

#ifndef __EuclidCommon_h__
#define __EuclidCommon_h__

//
#include "base/Buddha.h"
#include "math/Euler.h"
#include "external/directx/Include/d3d9.h"
#include "external/directx/Include/d3dx9.h"
//
namespace Euclid
{
	//
#if defined(render_EXPORTS) // inside DLL
#   define _EuclidExport_   __declspec(dllexport)
#else // outside DLL
#   define _EuclidExport_   __declspec(dllimport)
#endif

	// declearation classes
	struct IRenderSystem;
	struct Rect;
	class IMaterial;
	class IMaterialManager;
	class IBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class BufferManager;
	class Effect;
	class EffectManager;
	class Camera;
	class CameraControllerThirdPerson;
}

//
#endif // __EuclidCommon_h__
