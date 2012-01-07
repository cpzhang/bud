//==========================================================================
/**
* @file	  : FileSystem.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-20   11:04
* purpose : 
*/
//==========================================================================

#ifndef __FileSystem_h__
#define __FileSystem_h__
//
#include "Common.h"
#include "Singleton.h"
//
namespace Buddha
{
	class _BUHHDA_EXPORT_ FileSystem : public SingletonEx<FileSystem>
	{
	public:
		FileSystem();
		~FileSystem();
	public:
		void getWindowDirectory(std::string& dir);
		void getModuleFileName(std::string& file);
		void getDataDirectory(std::string& dir);
		std::string getDataDirectory();
		void getBinDirectory(std::string& dir);
		void getParent(std::string& dir);
		//
		bool isFileExist(std::string& fileName);
		std::string getFileExtension(const std::string& fileName);
	private:
	private:
		static const unsigned long Max_Path;
	};
}

#endif // __FileSystem_h__
 
