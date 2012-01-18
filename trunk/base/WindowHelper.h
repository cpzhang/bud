//==========================================================================
/**
* @file	  : WindowHelper.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-2   19:00
* purpose : 
*/
//==========================================================================

#ifndef __WindowHelper_h__
#define __WindowHelper_h__

//
#include "Common.h"
#include "Singleton.h"
//
namespace Buddha
{
	class _BUHHDA_EXPORT_ WindowHelper
	{
		//
	public:
		//
		static int getWindowHeight(HWND hwnd);

		//
		static int getWindowWidth(HWND hwnd);

		//
		static int getClientWidth(HWND hwnd);

		//
		static int getClientHeight(HWND hwnd);

		//
		static int getScreenWidth();

		//
		static int getScreenHeight();

		//
		static float getSceenWidthHeightRatio();
		//
		static void setAccelerationLevel(int level);
		//
		static int getAccelerationLevel();
	};
}

#endif // __WindowHelper_h__
 
