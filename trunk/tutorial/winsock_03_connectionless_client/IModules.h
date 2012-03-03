//==========================================================================
/**
* @file	  : IModules.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   20:58
* purpose : 
*/
//==========================================================================

#ifndef __IModules_h__
#define __IModules_h__

//
#include "render/Euclid.h"
#include "net/Shannon.h"

//
class IModules
{
public:
	virtual bool create(HWND hwnd) = 0;
	virtual Euclid::IRenderEngine* getRenderEngine() = 0;
	virtual void destroy() = 0;
};

#endif // __IModules_h__
