#include "CameraControllerThirdPerson.h"
#include "Camera.h"
namespace Euclid
{
	CameraControllerThirdPerson::CameraControllerThirdPerson( Camera *pCamera )
	{
		m_pCamera = pCamera;
		m_mtxWorld = Mat4::IDENTITY;
		m_bLButtonDown = false;
		m_bMButtonDown = false;
	}

	CameraControllerThirdPerson::CameraControllerThirdPerson()
	{
		m_pCamera = NULL;
		m_mtxWorld = Mat4::IDENTITY;
		m_bLButtonDown = false;
		m_bMButtonDown = false;
	}

	void CameraControllerThirdPerson::release()
	{
		delete this;
	}

	tstring CameraControllerThirdPerson::getName()
	{
		return "CameraControllerThirdPerson";
	}

	void CameraControllerThirdPerson::onKeyDown( char c )
	{
		switch(c)
		{
		case 'S':
		case 's':
		case VK_DOWN:
			m_pCamera->moveRelative(Vec3(0,0,2));
			break;
		case 'W':
		case 'w':
		case VK_UP:
			m_pCamera->moveRelative(Vec3(0,0,-2));
			break;
		case 'A':
		case 'a':
		case VK_LEFT:
			m_pCamera->moveRelative(Vec3(-2,0,0));
			break;
		case 'D':
		case 'd':
		case VK_RIGHT:
			m_pCamera->moveRelative(Vec3(2,0,0));
			break;
			//
		case 'R':
		case 'r':
			m_pCamera->moveRelative(Vec3(0,2,0));
			break;
		case 'F':
		case 'f':
			m_pCamera->moveRelative(Vec3(0,-2,0));
			break;
		}
	}

	void CameraControllerThirdPerson::onRightButtonDown( u16 x,u16 y )
	{

	}

	void CameraControllerThirdPerson::onMouseMove( u16 x,u16 y )
	{
		if(m_bLButtonDown)
		{
			float angleX = (float)(x - m_ptOrigin.x) * 0.1f;
			float angleY = (float)(y - m_ptOrigin.y) * 0.1f;
			Quaternion qy(angleX, Vec3::UNIT_Y);
			Mat4 mtxWorld(qy);
			m_mtxWorld = m_mtxWorld * mtxWorld;
			Mat4 mi = m_mtxWorld.inverse();

			Vec3 v = m_pCamera->getRight();
			v = mi * v;
			Quaternion q;
// 			std::ostringstream os;
// 			os<<angleY;
// 			Record(os.str());
			q.FromAngleAxis(angleY, v);
			m_mtxWorld = m_mtxWorld * q;
		}
		else if(m_bMButtonDown)
		{
			m_pCamera->moveRelative(Vec3((-x + m_ptOrigin.x) / 4.0f,0,0));
			m_pCamera->moveRelative(Vec3(0,(y - m_ptOrigin.y) / 4.0f,0));
		}

		m_ptOrigin.x = x;
		m_ptOrigin.y = y;
	}

	void CameraControllerThirdPerson::onRightButtonUp( u16 x,u16 y )
	{

	}

	void CameraControllerThirdPerson::onLeftButtonDown( u16 x,u16 y )
	{
		if(m_bLButtonDown)return;

		m_bLButtonDown = true;
		m_ptOrigin.x = x;
		m_ptOrigin.y = y;
	}

	void CameraControllerThirdPerson::onLeftButtonUp( u16 x,u16 y )
	{
		if(!m_bLButtonDown)return;

		m_bLButtonDown = false;
	}

	void CameraControllerThirdPerson::onMouseWheel( float delta )
	{
		m_pCamera->moveRelative(Vec3(0,0,-delta / 25.0f));
	}

	void CameraControllerThirdPerson::onMButtonDown( u16 x,u16 y )
	{
		m_bMButtonDown = true;
		m_ptOrigin.x = x;
		m_ptOrigin.y = y;
	}

	void CameraControllerThirdPerson::onMButtonUp( u16 x,u16 y )
	{
		m_bMButtonDown = false;
	}

	const Mat4& CameraControllerThirdPerson::getMatrix()
	{
		return m_mtxWorld;
	}

	void CameraControllerThirdPerson::attachCamera( Camera *pCamera )
	{
		m_pCamera = pCamera;
		m_mtxWorld = Mat4::IDENTITY;
		m_bLButtonDown = false;
		m_bMButtonDown = false;
	}

	void CameraControllerThirdPerson::update()
	{

	}

}