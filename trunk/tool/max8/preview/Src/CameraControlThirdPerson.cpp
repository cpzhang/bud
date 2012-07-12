#include "stdafx.h"
#include "CameraControlThirdPerson.h"

CameraControlThirdPerson::CameraControlThirdPerson(Camera *pCamera,IRenderSystem* pRenderSystem) : m_pCamera(pCamera),m_bLButtonDown(false),m_bMButtonDown(false),m_pRenderSystem(pRenderSystem)
{
	m_mtxWorld = rkt::Matrix4::IDENTITY;
}

void CameraControlThirdPerson::release()
{
	delete this;
}

std::string CameraControlThirdPerson::getName()
{
	return "第三人称模式";
}

void CameraControlThirdPerson::onKeyDown(char c)
{
	switch(c)
	{
	case 'S':
	case 's':
		m_pCamera->moveRelative(rkt::Vector3(0,0,2));
		break;
	case 'W':
	case 'w':
		m_pCamera->moveRelative(rkt::Vector3(0,0,-2));
		break;
	case 'A':
	case 'a':
		m_pCamera->moveRelative(rkt::Vector3(-2,0,0));
		break;
	case 'D':
	case 'd':
		m_pCamera->moveRelative(rkt::Vector3(2,0,0));
		break;
	}
}

void CameraControlThirdPerson::onRightButtonDown(ushort x,ushort y)
{
}

void CameraControlThirdPerson::onMouseMove(ushort x,ushort y)
{
	if(m_bLButtonDown)
	{
		Matrix4 mtxWorld;
		mtxWorld.rotationY(x - m_ptOrigin.x);
		m_mtxWorld = m_mtxWorld * mtxWorld;
		Matrix4 mi = m_mtxWorld.inverse();

		Vector3 v = m_pCamera->getRight();
		v = mi * v;
		Quaternion q;
		q.FromAngleAxis(y - m_ptOrigin.y,v);
		m_mtxWorld = m_mtxWorld * q;
	}
	else if(m_bMButtonDown)
	{
		m_pCamera->moveRelative(Vector3((-x + m_ptOrigin.x) / 4.0f,0,0));
		m_pCamera->moveRelative(Vector3(0,(y - m_ptOrigin.y) / 4.0f,0));
	}

	m_ptOrigin.x = x;
	m_ptOrigin.y = y;
}

void CameraControlThirdPerson::onRightButtonUp(ushort x,ushort y)
{
}

void CameraControlThirdPerson::onLeftButtonDown(ushort x,ushort y)
{
	if(m_bLButtonDown)return;
	
	m_bLButtonDown = true;
	m_ptOrigin.x = x;
	m_ptOrigin.y = y;
}

void CameraControlThirdPerson::onLeftButtonUp(ushort x,ushort y)
{
	if(!m_bLButtonDown)return;

	m_bLButtonDown = false;
}

void CameraControlThirdPerson::onMouseWheel(float delta)
{
	m_pCamera->moveRelative(Vector3(0,0,-delta / 25.0f));
}

void CameraControlThirdPerson::onMButtonDown(ushort x,ushort y)
{
	m_bMButtonDown = true;
	m_ptOrigin.x = x;
	m_ptOrigin.y = y;
}

void CameraControlThirdPerson::onMButtonUp(ushort x,ushort y)
{
	m_bMButtonDown = false;
}

const Matrix4& CameraControlThirdPerson::getWorldMatrix()
{
	return m_mtxWorld;
}