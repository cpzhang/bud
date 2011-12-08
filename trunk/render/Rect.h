/********************************************************************
**	file: 		Rect.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __Rect_h__
#define __Rect_h__

namespace Euclid
{
	struct Rect
	{
		long upper_left_x;
		long upper_left_y;

		long lower_right_x;
		long lower_right_y; 

		void toD3DRECT(D3DRECT& rt) const
		{
			ZeroMemory(&rt, sizeof(rt));
			rt.x1 = upper_left_x;
			rt.y1 = upper_left_y;

			rt.x2 = lower_right_x;
			rt.y2 = lower_right_y;
		}

		void toRECT(RECT& rt) const
		{
			ZeroMemory(&rt, sizeof(rt));
			rt.left = upper_left_x;
			rt.right = lower_right_x;

			rt.bottom = lower_right_y;
			rt.top = upper_left_y;
		}
	};
}


#endif // __Rect_h__