//==========================================================================
/**
* @file	  : Modules.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   21:00
* purpose : 
*/
//==========================================================================

#ifndef __Modules_h__
#define __Modules_h__

#include "IModules.h"

//
class Modules : public IModules
{
public:
	Modules();
	~Modules();
	
	//
public:
	//
	virtual bool create(HWND hwnd);

	//
	virtual Euclid::IRenderEngine* getRenderEngine();

	//
	virtual void destroy();

private:
	//
	Euclid::IRenderEngine*		_renderEngine;
};

#endif // __Modules_h__
 
