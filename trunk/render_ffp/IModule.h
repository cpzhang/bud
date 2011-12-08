//==========================================================================
/**
* @file	  : IModule.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-3-29   11:55
* purpose : 
*/
//==========================================================================

#ifndef __IModule_h__
#define __IModule_h__

namespace Euclid
{
	class IModule
	{
	public:
		virtual bool create()	= 0;
		virtual bool update()			= 0;	
		virtual void shutDown()		= 0;
	};
}

#endif // __IModule_h__
 
