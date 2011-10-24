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
	class _BUHHDA_EXPORT_ WindowHelper : public SingletonEx<WindowHelper>
	{
	public:
		WindowHelper();
		~WindowHelper();

		//
	public:
		//
		int getWindowHeight(HWND hwnd);

		//
		int getWindowWidth(HWND hwnd);

		//
		int getClientWidth(HWND hwnd);

		//
		int getClientHeight(HWND hwnd);

		//
		int getScreenWidth();

		//
		int getScreenHeight();

		//
		float getSceenWidthHeightRatio();
	};
}

#endif // __WindowHelper_h__
 
