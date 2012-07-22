//==========================================================================
/**
* @file	  : Logger.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-25   21:07
* purpose : 
*/
//==========================================================================

#ifndef __Logger_h__
#define __Logger_h__

#include "Common.h"
#include "Singleton.h"

namespace Zen
{
	class _BUHHDA_EXPORT_ Logger : public SingletonEx<Logger>
	{
	public:
		Logger();
		~Logger();

	public:
		enum eLevel
		{
			eLevel_Record,
			eLevel_Warning,
			eLevel_Error
		};

	public:
		void add(const char* content, eLevel level, bool newLine = true);
		void add(std::ostringstream& content, eLevel level, bool newLine = true);
		void add(int content, eLevel level, bool newLine = true);
		void addDate();
		tstring getPath();
	private:
		std::ofstream		_log;
		tstring			_path;
	};

#define Record(content)\
	Zen::Logger::getInstancePtr()->addDate();\
	Zen::Logger::getInstancePtr()->add(__FILE__, Zen::Logger::eLevel_Record, false);\
	Zen::Logger::getInstancePtr()->add(__LINE__, Zen::Logger::eLevel_Record);\
	Zen::Logger::getInstancePtr()->add(content, Zen::Logger::eLevel_Record);


#define Warning(content)\
	Zen::Logger::getInstancePtr()->addDate();\
	Zen::Logger::getInstancePtr()->add(__FILE__, Zen::Logger::eLevel_Warning, false);\
	Zen::Logger::getInstancePtr()->add(__LINE__, Zen::Logger::eLevel_Warning);\
	Zen::Logger::getInstancePtr()->add(content, Zen::Logger::eLevel_Warning);


#define Error(content)\
	Zen::Logger::getInstancePtr()->addDate();\
	Zen::Logger::getInstancePtr()->add(__FILE__, Zen::Logger::eLevel_Error, false);\
	Zen::Logger::getInstancePtr()->add(__LINE__, Zen::Logger::eLevel_Error);\
	Zen::Logger::getInstancePtr()->add(content, Zen::Logger::eLevel_Error);
}
#endif // __Logger_h__
 
