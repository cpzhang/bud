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
#include <windows.h>
#include <windowsx.h>
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

//# pragma pack(pop)

#define safeDestroy(x) if((x)!=NULL){ (x)->destroy(); delete (x); (x) = NULL;}
#define safeDelete(x) if((x)!=NULL){delete (x); (x) = NULL;}

#define safeDeleteArray(x) if((x)!=NULL){delete[] (x); (x) = NULL;}

namespace Buddha
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

//
#endif // __BaseCommon_h__
 
