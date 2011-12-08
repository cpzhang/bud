#include "CameraEx.h"
#include "stdafx.h"
#include "D3D9Mapping.h"
#include "Paras.h"
#include "IRenderSystem.h"
namespace Euclid
{
	CameraEx::CameraEx()
	{
		m_vPosition = Vec3::ZERO;
		m_qtnOrientation = Quaternion::IDENTITY;
		m_fNearDistance = 0.1f;
		m_fFarDistance = 10000.0;
		m_radFOVy = Radian(Euler::PI / 4.0f);
		m_dirty = true;
		m_mtxView = Mat4::IDENTITY;
		m_mtxProjection = Mat4::IDENTITY;
		m_mtxOrthoProjection = Mat4::IDENTITY;
		_ctp = NULL;
	}

	const Mat4& CameraEx::getViewMatrix()
	{
		return m_mtxView;
	}


	const Mat4& CameraEx::getProjectionMatrix()
	{
		return m_mtxProjection;
	}

	void CameraEx::calcViewMatrix()
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

	void CameraEx::calcProjectionMatrix()
	{
		D3DXMATRIX m;
		D3DVIEWPORT9 vp;
		Paras::getInstancePtr()->_renderSystem->getViewport(&vp);
		float width = vp.Width;
		float height = vp.Height;

		D3DXMatrixPerspectiveFovRH(&m, /*m_radFOVy._radian*/D3DX_PI * 0.45, width / height, m_fNearDistance, m_fFarDistance);
		m_mtxProjection = D3D9Mapping::convertD3DXMatrix(m);
	}


	void CameraEx::calcOrthoProjectionMatrix()
	{
		D3DXMATRIX m;
		D3DVIEWPORT9 vp;
		Paras::getInstancePtr()->_renderSystem->getViewport(&vp);
		float width = vp.Width;
		float height = vp.Height;

		D3DXMatrixOrthoRH(&m, width, height, m_fNearDistance, m_fFarDistance);
		m_mtxOrthoProjection = D3D9Mapping::convertD3DXMatrix(m);
	}

	void CameraEx::roll( const Radian& angle )
	{
		// Rotate around local Z axis
		Vec3 zAxis = m_qtnOrientation * Vec3::UNIT_Z;
		rotate(zAxis, angle);
	}

	void CameraEx::yaw( const Radian& angle )
	{
		// Rotate around local Y axis
		Vec3 yAxis = m_qtnOrientation * Vec3::UNIT_Y;
		rotate(yAxis, angle);
	}

	void CameraEx::pitch( const Radian& angle )
	{
		// Rotate around local X axis
		Vec3 xAxis = m_qtnOrientation * Vec3::UNIT_X;
		rotate(xAxis,angle);
	}

	void CameraEx::rotate( const Vec3& axis, const Radian& angle )
	{
		Quaternion q;
		q.FromAngleAxis(angle,axis);
		rotate(q);
	}

	void CameraEx::rotate( const Quaternion& q )
	{
		// Note the order of the mult, i.e. q comes after
		// Normalise the quat to avoid cumulative problems with precision
		Quaternion qnorm = q;
		qnorm.normalize();
		m_qtnOrientation = qnorm * m_qtnOrientation;
		m_dirty = true;
	}

	void CameraEx::setPosition( const Vec3& vec )
	{
		m_vPosition = vec;
		m_dirty = true;
	}

	void CameraEx::move( const Vec3& vec )
	{
		m_vPosition += vec;
		m_dirty = true;
	}

	void CameraEx::moveRelative( const Vec3& vec )
	{
		// Transform the axes of the relative vector by camera's local axes
		Vec3 trans = m_qtnOrientation * vec;
		m_vPosition += trans;
		m_dirty = true;
	}

	void CameraEx::setDirection( const Vec3& vec )
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

	void CameraEx::lookAt( const Vec3& targetPoint )
	{
		setDirection(targetPoint - m_vPosition);
		m_dirty = true;
	}

	void CameraEx::setNearClipDistance( Real nearDist )
	{
		m_fNearDistance = nearDist;
		m_dirty = true;
	}

	void CameraEx::setFarClipDistance( Real farDist )
	{
		m_fNearDistance = farDist;
		m_dirty = true;
	}

	void CameraEx::update()
	{
		if (m_dirty)
		{
			calcViewMatrix();
			calcProjectionMatrix();
			calcOrthoProjectionMatrix();
			m_dirty = false;
		}
	}

	Vec3 CameraEx::getDirection( void ) const
	{
		// Direction points down -Z by default
		return m_qtnOrientation * Vec3::NEGATIVE_UNIT_Z;
	}

	Vec3 CameraEx::getUp( void ) const
	{
		return m_qtnOrientation * Vec3::UNIT_Y;
	}

	Vec3 CameraEx::getRight( void ) const
	{
		return m_qtnOrientation * Vec3::UNIT_X;
	}

	void CameraEx::setControllerThirdPerson( CameraControllerThirdPerson* ctp )
	{
		_ctp = ctp;
	}

	CameraControllerThirdPerson* CameraEx::getControllerThirdPerson()
	{
		return _ctp;
	}

	const Mat4& CameraEx::getOrthoProjectionMatrix()
	{
		return m_mtxOrthoProjection;
	}

}