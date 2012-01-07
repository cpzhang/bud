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

	void FileSystem::getWindowDirectory( std::string& dir )
	{
		dir.resize(Max_Path);
		::GetWindowsDirectory((LPSTR)(dir.c_str()), Max_Path);
	}

	void FileSystem::getModuleFileName( std::string& file )
	{
		file.resize(Max_Path);
		::GetModuleFileName(NULL, (LPSTR)(file.c_str()), Max_Path);
	}

	void FileSystem::getDataDirectory( std::string& dir )
	{
		getModuleFileName(dir);
		std::string frag("\\bin\\");
		size_t pos = dir.find(frag);
		if (pos != std::string::npos)
		{
			dir.replace(pos + 1, dir.size() - 1, "data");
		}
		dir += "\\";
	}

	std::string FileSystem::getDataDirectory()
	{
		static std::string dir;
		if (dir.empty())
		{
			getDataDirectory(dir);
		}
		return dir;
	}

	bool FileSystem::isFileExist( std::string& fileName )
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

	void FileSystem::getBinDirectory( std::string& dir )
	{
		getModuleFileName(dir);
		
		size_t pos = dir.find_last_of('\\');
		if (pos != std::string::npos)
		{
			dir = dir.substr(0, pos);
		}
	}

	void FileSystem::getParent( std::string& dir )
	{
		size_t pos = dir.find_last_of('\\');
		if (pos != std::string::npos)
		{
			dir = dir.substr(0, pos);
		}
	}

	std::string FileSystem::getFileExtension( const std::string& fileName )
	{
		size_t pos = fileName.find_last_of('.');
		if (pos != std::string::npos)
		{
			return fileName.substr(pos + 1, fileName.size() - pos - 1);
		}
		return "";
	}

	const unsigned long FileSystem::Max_Path = 300;
}
