#include "Logger.h" 
#include "FileSystem.h"

namespace Buddha
{
	Logger::Logger()
	{
		tstring path;
		FileSystem::getInstancePtr()->getBinDirectory(path);
		path += TEXT("\\log.html");
		
		_path = path;
		_log.open(path.c_str());

		_log<<"<html><body style=\"background-color:rgb(255,255,255)\"><p>";
	}

	Logger::~Logger()
	{
		_log<<"</p></body></html>";
		_log.close();
	}

	void Logger::add( const char* content, eLevel level, bool newLine /*= true*/ )
	{
		switch(level)
		{
		case eLevel_Record:
			_log<<"<font size=\"2\" face=\"Times\" color=black>";
			break;

		case eLevel_Warning:
			_log<<"<font size=\"2\" face=\"Times\" color=blue>";
			break;

		case eLevel_Error:
			_log<<"<font size=\"2\" face=\"Times\" color=red>";
			break;

		default:
			break;
		}

		if (newLine)
		{
			_log<<content<<"</br></font>";
		}
		else
		{
			_log<<content<<"</font>  ";
		}
	}

	void Logger::add( int content, eLevel level, bool newLine /*= true*/  )
	{
		std::ostringstream buf;
		buf<<content;

		add(buf.str().c_str(), level, newLine);
	}

	void Logger::add(std::ostringstream& content, eLevel level, bool newLine /*= true*/ )
	{
		add(content.str().c_str(), level, newLine);
	}

	void Logger::addDate()
	{
		SYSTEMTIME t;
		GetLocalTime(&t);

		_log<<"["<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay<<" "<<t.wHour<<":"<<t.wMinute<<":"<<t.wSecond<<"]";
	}

	tstring Logger::getPath()
	{
		return _path;
	}
}	