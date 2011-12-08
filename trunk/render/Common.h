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
}

//
#endif // __EuclidCommon_h__
