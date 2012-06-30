#include "FileSystem.h"
//
namespace Buddha
{
	FileSystem::FileSystem()
	{

	}

	FileSystem::~FileSystem()
	{

	}

	void FileSystem::getWindowDirectory( tstring& dir )
	{
		dir.resize(Max_Path);
		::GetWindowsDirectory((LPTSTR)dir.c_str(), Max_Path);
	}

	void FileSystem::getModuleFileName( tstring& file )
	{
		file.resize(Max_Path);
		::GetModuleFileName(NULL, (LPTSTR)(file.c_str()), Max_Path);
	}

	void FileSystem::getDataDirectory( tstring& dir )
	{
		getModuleFileName(dir);
		tstring frag(TEXT("\\bin\\"));
		size_t pos = dir.rfind(frag);
		if (pos != tstring::npos)
		{
			dir.replace(pos + 1, dir.size() - 1, TEXT("data"));
		}
		dir += TEXT("\\");
	}

	tstring FileSystem::getDataDirectory()
	{
		static tstring dir;
		if (dir.empty())
		{
			getDataDirectory(dir);
		}
		return dir.c_str();
	}

	bool FileSystem::isFileExist( tstring& fileName )
	{
		bool exist = false;
		std::ifstream f(fileName.c_str(), std::ios_base::binary);

		//
		if (f.good())
		{
			exist = true;
		}
		
		//
		f.close();

		//
		return exist;
	}

	void FileSystem::getBinDirectory( tstring& dir )
	{
		getModuleFileName(dir);
		
		size_t pos = dir.find_last_of('\\');
		if (pos != tstring::npos)
		{
			dir = dir.substr(0, pos);
		}
	}

	tstring FileSystem::getParent( const tstring& dir )
	{
		tstring path(dir);
		if (path[path.size() -1] == '/')
		{
			path = path.substr(0, path.size() - 1);
		}
		size_t pos = path.find_last_of('\\');
		if (pos == tstring::npos)
		{
			pos = path.find_last_of('/');
		}

		if (pos != tstring::npos)
		{
			return path.substr(0, pos);
		}
		return TEXT("");
	}

	tstring FileSystem::getFileExtension( const tstring& fileName )
	{
		size_t pos = fileName.find_last_of('.');
		if (pos != tstring::npos)
		{
			return fileName.substr(pos + 1, fileName.size() - pos - 1);
		}
		return TEXT("");
	}

	tstring FileSystem::removeParent( const tstring& dir )
	{
		size_t pos = dir.find_last_of('\\');
		if (pos == tstring::npos)
		{
			pos = dir.find_last_of('/');
		}
		
		if (pos != tstring::npos)
		{
			return dir.substr(pos + 1, dir.size() - pos - 1);
		}
		return dir;
	}

	tstring FileSystem::removeFileExtension( const tstring& fileName )
	{
		size_t pos = fileName.find_last_of('.');
		if (pos != tstring::npos)
		{
			return fileName.substr(0, pos);
		}
		return fileName;
	}

	tstring FileSystem::standardFilePath(const tstring& path)
	{
		tstring p = path;
		for (size_t i = 0; i != path.size(); ++i)
		{
			if (path[i] == '\\')
			{
				p[i] = '/';
			}
		}
		return p;
	}
	void FileSystem::createFolder(const tstring& path)
	{
		tstring folder = standardFilePath(path);
		size_t pos = folder.find_first_of(TEXT("/"));
		while(pos != tstring::npos)
		{
			tstring f = folder.substr(0, pos);
			CreateDirectory(f.c_str(), NULL);
			pos = folder.find_first_of(TEXT("/"), pos + 1);
		}
	}
	const unsigned long FileSystem::Max_Path = 300;
}
