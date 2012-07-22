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
namespace Zen
{
	class _BUHHDA_EXPORT_ FileSystem : public SingletonEx<FileSystem>
	{
	public:
		FileSystem();
		~FileSystem();
	public:
		void getWindowDirectory(tstring& dir);
		void getModuleFileName(tstring& file);
		void getDataDirectory(tstring& dir);
		tstring getDataDirectory();
		void getBinDirectory(tstring& dir);
		tstring getParent(const tstring& dir);
		tstring removeParent(const tstring& dir);
		//
		bool isFileExist(tstring& fileName);
		tstring getFileExtension(const tstring& fileName);
		tstring removeFileExtension(const tstring& fileName);
		tstring standardFilePath(const tstring& path);
		void createFolder(const tstring& path);
	private:
	private:
		static const unsigned long Max_Path;
	};
}

#endif // __FileSystem_h__
 
