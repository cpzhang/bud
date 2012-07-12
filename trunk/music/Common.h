#ifndef __Common_h__
#define __Common_h__
//
#if defined(music_EXPORTS) // inside DLL
#   define _MUSIC_EXPORT_   __declspec(dllexport)
#else // outside DLL
#   define _MUSIC_EXPORT_   __declspec(dllimport)
#endif

//
#include "base/Buddha.h"

//
namespace ABing
{
	class IMusicEngine;
	class MusicEngine;
}

#endif //__Common_h__
