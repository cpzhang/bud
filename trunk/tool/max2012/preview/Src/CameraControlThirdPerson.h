#ifndef __CameraControlThirdPerson_H__
#define __CameraControlThirdPerson_H__

#include "ICameraControl.h"

class CameraControlThirdPerson : public ICameraControl
{
public:
	/** ICameraControl */
	CameraControlInterface(;)
public:
	CameraControlThirdPerson(Camera *pCamera,IRenderSystem* pRenderSystem);
private:
	Camera*		m_pCamera;
	bool		m_bLButtonDown;
	POINT		m_ptOrigin;
	bool		m_bMButtonDown;
	IRenderSystem*	m_pRenderSystem;
	Matrix4		m_mtxWorld;
};

#endif