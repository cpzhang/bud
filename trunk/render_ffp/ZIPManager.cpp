#include "ZIPManager.h"
#include "PK3Archive.h"
#include "base/Logger.h"

namespace Euclid
{
	ZIPManager::ZIPManager()
	{
		Record("ZIPManager constructor");
	}

	ZIPManager::~ZIPManager()
	{
		Record("ZIPManager descontructor");
	}

	bool ZIPManager::addZIP( const std::string& zipPath )
	{
		if (_zips.find(zipPath) != _zips.end())
		{
			return false;
		}

		PK3Archive* zip = new PK3Archive;
		if (zip->create(zipPath))
		{
			_zips[zipPath] = zip;
			return true;
		}

		return true;
	}

	IFileRead* ZIPManager::getFileReader( const std::string& name )
	{
		IFileRead* f = NULL;

		ZIPMap::iterator it;
		for (it = _zips.begin(); it != _zips.end(); ++it)
		{
			f = it->second->getFileReader(name);
			if (f != NULL)
			{
				break;
			}
		}

		return f;
	}
}
