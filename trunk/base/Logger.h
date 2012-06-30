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

namespace Buddha
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
	Buddha::Logger::getInstancePtr()->addDate();\
	Buddha::Logger::getInstancePtr()->add(__FILE__, Buddha::Logger::eLevel_Record, false);\
	Buddha::Logger::getInstancePtr()->add(__LINE__, Buddha::Logger::eLevel_Record);\
	Buddha::Logger::getInstancePtr()->add(content, Buddha::Logger::eLevel_Record);


#define Warning(content)\
	Buddha::Logger::getInstancePtr()->addDate();\
	Buddha::Logger::getInstancePtr()->add(__FILE__, Buddha::Logger::eLevel_Warning, false);\
	Buddha::Logger::getInstancePtr()->add(__LINE__, Buddha::Logger::eLevel_Warning);\
	Buddha::Logger::getInstancePtr()->add(content, Buddha::Logger::eLevel_Warning);


#define Error(content)\
	Buddha::Logger::getInstancePtr()->addDate();\
	Buddha::Logger::getInstancePtr()->add(__FILE__, Buddha::Logger::eLevel_Error, false);\
	Buddha::Logger::getInstancePtr()->add(__LINE__, Buddha::Logger::eLevel_Error);\
	Buddha::Logger::getInstancePtr()->add(content, Buddha::Logger::eLevel_Error);
}
#endif // __Logger_h__
 
