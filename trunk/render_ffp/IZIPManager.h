//==========================================================================
/**
* @file	  : IZIPManager.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-20   16:53
* purpose : 
*/
//==========================================================================

#ifndef __IZIPManager_h__
#define __IZIPManager_h__

#include "Common.h"

namespace Euclid
{
	class IZIPManager
	{
	public:
		virtual ~IZIPManager(){};
	public:
		virtual bool addZIP(const std::string& zipPath) = 0;
		virtual IFileRead* getFileReader(const std::string& name) = 0;
	};
}

#endif // __IZIPManager_h__
