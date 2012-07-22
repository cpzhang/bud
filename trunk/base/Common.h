//==========================================================================
/**
* @file	  : BaseCommon.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-24   11:58
* purpose : 
*/
//==========================================================================

#ifndef __BaseCommon_h__
#define __BaseCommon_h__

//
#if defined(base_EXPORTS) // inside DLL
#   define _BUHHDA_EXPORT_   __declspec(dllexport)
#else // outside DLL
#   define _BUHHDA_EXPORT_   __declspec(dllimport)
#endif

//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include <windowsx.h>
#include <MMSystem.h>
#include <DbgHelp.h>
//
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <hash_map>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

//
#include <tchar.h>
#include <strsafe.h>
//
#ifdef  UNICODE                     
	#define tstring std::wstring      
#else   /* UNICODE */               
	#define tstring std::string
#endif /* UNICODE */ 

#pragma warning (disable : 4251)

//#	pragma pack( push, 1 )

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed __int64 s64;
typedef unsigned __int64 u64;

//
typedef float Real;

//# pragma pack(pop)

#define safeDestroy(x) if((x)!=NULL){ (x)->destroy(); delete (x); (x) = NULL;}
#define safeDelete(x) if((x)!=NULL){delete (x); (x) = NULL;}

#define safeDeleteArray(x) if((x)!=NULL){delete[] (x); (x) = NULL;}

namespace Zen
{
	//
#define breakable switch(1) case 1: default:

	class wstring;
	class FileSystem;
	class Mouse;
	class WindowHelper;
	class Logger;
	class MemoryFileWriter;
	class ChunkSet;
	template<class T> class CSVReader;
	template<class T> class SingletonEx;
}
/**
 * \def MaxCStringSize
 *
 * The maximum size allowed for C string (zero terminated string) buffer.
 * This value is used when we have to create a standard C string buffer and we don't know exactly the final size of the string.
 */
const int MaxCStringSize = 2048;


/**
 * \def NLMISC_CONVERT_VARGS(dest,format)
 *
 * This macro converts variable arguments into C string (zero terminated string).
 * This function takes care to avoid buffer overflow.
 *
 * Example:
 *\code
	void MyFunction(const char *format, ...)
	{
		string str;
		NLMISC_CONVERT_VARGS (str, format, NLMISC::MaxCStringSize);
		// str contains the result of the conversion
	}
 *\endcode
 *
 * \param _dest \c string or \c char* that contains the result of the convertion
 * \param _format format of the string, it must be the last argument before the \c '...'
 * \param _size size of the buffer that will contain the C string
 */
#define Convert_Vargs2String(_dest,_format,_size) \
char _cstring[_size]; \
va_list _args; \
va_start (_args, _format); \
int _res = vsnprintf (_cstring, _size-1, _format, _args); \
if (_res == -1 || _res == _size-1) \
{ \
	_cstring[_size-1] = '\0'; \
} \
va_end (_args); \
_dest = _cstring

//
#endif // __BaseCommon_h__
 
