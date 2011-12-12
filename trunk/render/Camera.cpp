#include "Camera.h"
#include "DXMapping.h"
#include "RenderSystem.h"
namespace Euclid
{
	Camera::Camera()
	{
		m_vPosition = Vec3::ZERO;
		m_qtnOrientation = Quaternion::IDENTITY;
		m_fNearDistance = 1.0f;
		m_fFarDistance = 10000.0;
		m_radFOVy = Radian(Euler::PI / 4.0f);
		m_dirty = true;
		m_mtxView = Mat4::IDENTITY;
		m_mtxProjection = Mat4::IDENTITY;
		m_mtxOrthoProjection = Mat4::IDENTITY;
		_ctp = NULL;
	}

	const Mat4& Camera::getViewMatrix()
	{
		return m_mtxView;
	}


	const Mat4& Camera::getProjectionMatrix()
	{
		return m_mtxProjection;
	}

	void Camera::calcViewMatrix()
	{
		// View matrix is:
		//
		//  [ Lx  Uy  Dz  Tx  ]
		//  [ Lx  Uy  Dz  Ty  ]
		//  [ Lx  Uy  Dz  Tz  ]
		//  [ 0   0   0   1   ]
		//
		// Where T = -(Transposed(Rot) * Pos)

		// This is most efficiently done using 3x3 Matrices
		Mat3 rot;
		m_qtnOrientation.ToRotationMatrix(rot);

		// Make the translation relative to new axes
		Mat3 rotT = rot.Transpose();
		Vec3 trans = -rotT * m_vPosition;

		// Make final matrix
		m_mtxView = Mat4::IDENTITY;
		m_mtxView = rotT; // fills upper 3x3
		m_mtxView[0][3] = trans.x;
		m_mtxView[1][3] = trans.y;
		m_mtxView[2][3] = trans.z;
	}

	void Camera::calcProjectionMatrix()
	{
		D3DXMATRIX m;
		D3DVIEWPORT9 vp;
		RenderSystem::getInstancePtr()->getViewPort(&vp);
		float width = vp.Width;
		float height = vp.Height;

		D3DXMatrixPerspectiveFovRH(&m, /*m_radFOVy._radian*/D3DX_PI * 0.45, width / height, m_fNearDistance, m_fFarDistance);
		m_mtxProjection = DXMapping::fromDXMatrix(m);
	}


	void Camera::calcOrthoProjectionMatrix()
	{
		D3DXMATRIX m;
		D3DVIEWPORT9 vp;
		RenderSystem::getInstancePtr()->getViewPort(&vp);
		float width = vp.Width;
		float height = vp.Height;

		D3DXMatrixOrthoRH(&m, width, height, m_fNearDistance, m_fFarDistance);
		m_mtxOrthoProjection = DXMapping::fromDXMatrix(m);
	}

	void Camera::roll( const Radian& angle )
	{
		// Rotate around local Z axis
		Vec3 zAxis = m_qtnOrientation * Vec3::UNIT_Z;
		rotate(zAxis, angle);
	}

	void Camera::yaw( const Radian& angle )
	{
		// Rotate around local Y axis
		Vec3 yAxis = m_qtnOrientation * Vec3::UNIT_Y;
		rotate(yAxis, angle);
	}

	void Camera::pitch( const Radian& angle )
	{
		// Rotate around local X axis
		Vec3 xAxis = m_qtnOrientation * Vec3::UNIT_X;
		rotate(xAxis,angle);
	}

	void Camera::rotate( const Vec3& axis, const Radian& angle )
	{
		Quaternion q;
		q.FromAngleAxis(angle,axis);
		rotate(q);
	}

	void Camera::rotate( const Quaternion& q )
	{
		// Note the order of the mult, i.e. q comes after
		// Normalise the quat to avoid cumulative problems with precision
		Quaternion qnorm = q;
		qnorm.normalize();
		m_qtnOrientation = qnorm * m_qtnOrientation;
		m_dirty = true;
	}

	void Camera::setPosition( const Vec3& vec )
	{
		m_vPosition = vec;
		m_dirty = true;
	}

	void Camera::move( const Vec3& vec )
	{
		m_vPosition += vec;
		m_dirty = true;
	}

	void Camera::moveRelative( const Vec3& vec )
	{
		// Transform the axes of the relative vector by camera's local axes
		Vec3 trans = m_qtnOrientation * vec;
		m_vPosition += trans;
		m_dirty = true;
	}

	void Camera::setDirection( const Vec3& vec )
	{
		// Do nothing if given a zero vector
		// (Replaced assert since this could happen with auto tracking camera and
		//  camera passes through the lookAt point)
		if (vec == Vec3::ZERO) return;

		// Remember, camera points down -Z of local axes!
		// Therefore reverse direction of direction vector before determining local Z
		Vec3 zAdjustVec = -vec;
		zAdjustVec.normalise();

		Vec3 mYawFixedAxis = Vec3(0,1,0);
		Vec3 xVec = mYawFixedAxis.crossProduct( zAdjustVec );
		xVec.normalise();

		Vec3 yVec = zAdjustVec.crossProduct( xVec );
		yVec.normalise();

		m_qtnOrientation.FromAxes( xVec, yVec, zAdjustVec );
		m_dirty = true;
	}

	void Camera::lookAt( const Vec3& targetPoint )
	{
		setDirection(targetPoint - m_vPosition);
		m_dirty = true;
	}

	void Camera::setNearClipDistance( Real nearDist )
	{
		m_fNearDistance = nearDist;
		m_dirty = true;
	}

	void Camera::setFarClipDistance( Real farDist )
	{
		m_fNearDistance = farDist;
		m_dirty = true;
	}

	void Camera::update()
	{
		if (m_dirty)
		{
			calcViewMatrix();
			calcProjectionMatrix();
			calcOrthoProjectionMatrix();
			m_dirty = false;
		}
	}

	Vec3 Camera::getDirection( void ) const
	{
		// Direction points down -Z by default
		return m_qtnOrientation * Vec3::NEGATIVE_UNIT_Z;
	}

	Vec3 Camera::getUp( void ) const
	{
		return m_qtnOrientation * Vec3::UNIT_Y;
	}

	Vec3 Camera::getRight( void ) const
	{
		return m_qtnOrientation * Vec3::UNIT_X;
	}

	void Camera::setControllerThirdPerson( CameraControllerThirdPerson* ctp )
	{
		_ctp = ctp;
	}

	CameraControllerThirdPerson* Camera::getControllerThirdPerson()
	{
		return _ctp;
	}

	const Mat4& Camera::getOrthoProjectionMatrix()
	{
		return m_mtxOrthoProjection;
	}

}