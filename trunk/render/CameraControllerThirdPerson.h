/********************************************************************
**	file: 		CameraControllerThirdPerson.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-6-2
**	purpose:	
*********************************************************************/
#ifndef __CameraControllerThirdPerson_h__
#define __CameraControllerThirdPerson_h__

#include "Common.h"

namespace Euclid
{
	class _EuclidExport_ CameraControllerThirdPerson
	{
	public:
		CameraControllerThirdPerson();
		CameraControllerThirdPerson(Camera *pCamera);
	public:
		void attachCamera(Camera *pCamera);
		virtual void release();

		virtual tstring getName();
		
		virtual void onKeyDown(char c);
		
		virtual void onRightButtonDown(u16 x,u16 y);
		
		virtual void onMouseMove(u16 x,u16 y);
		
		virtual void onRightButtonUp(u16 x,u16 y);
		
		virtual void onLeftButtonDown(u16 x,u16 y);
		
		virtual void onLeftButtonUp(u16 x,u16 y);
		
		virtual void onMouseWheel(float delta);
		
		virtual void onMButtonDown(u16 x,u16 y);
		
		virtual void onMButtonUp(u16 x,u16 y);
		
		virtual const Mat4& getMatrix();

		void update();
	private:
		Camera*	m_pCamera;
		bool		m_bLButtonDown;
		POINT		m_ptOrigin;
		bool		m_bMButtonDown;
		Mat4		m_mtxWorld;
	};
}

#endif // __CameraControllerThirdPerson_h__
