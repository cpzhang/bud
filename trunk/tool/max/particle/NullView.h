/********************************************************************
**	file: 		NullView.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-26
**	purpose:	
*********************************************************************/
#ifndef __NullView_h__
#define __NullView_h__

#include "MaxParticleCommon.h"

class NullView: public View 
{
public:
	Point2 ViewToScreen(Point3 p) 
	{
		return Point2(p.x,p.y); 
	}
	
	NullView()
	{
		worldToView.IdentityMatrix();
		screenW=640.0f; 
		screenH = 480.0f; 
	}
};


#endif // __NullView_h__
