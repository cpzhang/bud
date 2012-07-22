 //==========================================================================
 /**
 * @file	  : Mouse.h
 * @author : cpzhang <chengpengzhang@gmail.com>
 * created : 2010-5-2   17:52
 * purpose : 
 */
 //==========================================================================
 
 #ifndef __Mouse_h__
 #define __Mouse_h__
 
//
#include "Common.h"
#include "Singleton.h"
 namespace Zen
 {
	 class _BUHHDA_EXPORT_ Mouse : public SingletonEx<Mouse>
	 {
	 public:
		 Mouse();
		 ~Mouse();

	 public:
		 //
		 bool	isLeftButtonDown();

		 //
		 bool	isRightButtonDown();

		 //
		 POINT	getAbsPostion();

		 //	return the mouse postion relative to the client area of the window
		 POINT	getRelPosition(HWND hwnd);

		 //
		 POINT	getMoveLeftButtonDown();

		 //
		 POINT	getMoveRightButtonDown();

		 //
		 bool	update();

		 //
		 bool	isKeyDown(const char key);

		 //
	 private:
		 //
		 POINT		_point;
		 POINT		_pointLeftButtonDown;
		 POINT		_pointRightButtonDown;
	 };
 }
 
 #endif // __Mouse_h__
