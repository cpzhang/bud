//==========================================================================
/**
* @file	  : ZIPManager.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-20   16:54
* purpose : 
*/
//==========================================================================

#ifndef __ZIPManager_h__
#define __ZIPManager_h__

#include "Common.h"
#include "IZIPManager.h"

namespace Euclid
{
	class ZIPManager : public IZIPManager, public Buddha::SingletonEx<ZIPManager>
	{
	public:
		virtual bool addZIP(const std::string& zipPath);
		IFileRead* getFileReader(const std::string& name);
	public:
		ZIPManager();
		~ZIPManager();

	private:
		typedef std::map<std::string, PK3Archive*> ZIPMap;
		ZIPMap	_zips;
	};
}

#endif // __ZIPManager_h__
 
