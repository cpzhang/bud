#include "camera.h" 
#include "FontFactory.h"
//
namespace Euclid
{
	Camera::Camera()
	{
		_cameraType = Camera_AirCraft;
		//
		_eye	= D3DXVECTOR3(0.0f, 0.0f, 250.0f);
		_right	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		_up		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//	_lookAt is a vector, indicating which direction the eye ray points
		_lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	Camera::Camera( CameraType ct )
	{
		_cameraType = ct;
		//
		_eye	= D3DXVECTOR3(0.0f, 0.0f, 10.0f);
		_right	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		_up		= D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//	_lookAt is a vector, indicating which direction the eye ray points
		_lookAt = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::setPosition( float x, float y, float z )
	{
		_eye.x = x;
		_eye.y = y;
		_eye.z = z;
	}

	void Camera::lookAt( float x, float y, float z )
	{
		_lookAt = D3DXVECTOR3(x, y, z);
	}
	
	void Camera::strafe( float units )
	{
		switch(_cameraType)
		{
		case Camera_AirCraft:
			_eye += _right * units;
			break;
		case Camera_LandObject:
			_eye += D3DXVECTOR3(_right.x, 0, _right.z) * units;
			break;
		}
	}

	void Camera::fly( float units )
	{
		switch(_cameraType)
		{
		case Camera_AirCraft:
			_eye += _up * units;
			break;
		case Camera_LandObject:
			_eye.y +=  units;
			break;
		}
	}

	void Camera::walk( float units )
	{
		switch(_cameraType)
		{
		case Camera_AirCraft:
			_eye += _lookAt * units;
			break;
		case Camera_LandObject:
			_eye += D3DXVECTOR3(_lookAt.x, 0, _lookAt.z) * units;
			break;
		}
	}

	void Camera::pitch( float angle )
	{
		D3DXMATRIX m;
		D3DXMatrixRotationAxis(&m, &_right, angle);
		//
		D3DXVec3TransformCoord(&_up, &_up, &m);
		D3DXVec3TransformCoord(&_lookAt, &_lookAt, &m);
	}

	void Camera::yaw( float angle )
	{
		D3DXMATRIX T;

		// rotate around world y (0, 1, 0) always for land object
		if( _cameraType == Camera_LandObject )
			D3DXMatrixRotationY(&T, angle);

		// rotate around own up vector for aircraft
		if( _cameraType == Camera_AirCraft )
			D3DXMatrixRotationAxis(&T, &_up, angle);

		// rotate _right and _look around _up or y-axis
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_lookAt,&_lookAt, &T);

	}

	void Camera::roll( float angle )
	{	
		// only roll for aircraft type
		if( _cameraType == Camera_AirCraft)
		{
			D3DXMATRIX T;
			D3DXMatrixRotationAxis(&T, &_lookAt, angle);

			// rotate _up and _right around _look vector
			D3DXVec3TransformCoord(&_right,&_right, &T);
			D3DXVec3TransformCoord(&_up,&_up, &T);
		}
	}

	D3DXMATRIX* Camera::getViewMatrix( D3DXMATRIX& vm )
	{
		// Keep camera's axes orthogonal to eachother
		// z
		D3DXVec3Normalize(&_lookAt, &_lookAt);

		// y
		D3DXVec3Cross(&_up, &_lookAt, &_right);
		D3DXVec3Normalize(&_up, &_up);

		// x
		D3DXVec3Cross(&_right, &_up, &_lookAt);
		D3DXVec3Normalize(&_right, &_right);

		// Build the view matrix:
		float x = -D3DXVec3Dot(&_right, &_eye);
		float y = -D3DXVec3Dot(&_up, &_eye);
		float z = -D3DXVec3Dot(&_lookAt, &_eye);

		//
		(vm)(0,0) = _right.x; (vm)(0, 1) = _up.x; (vm)(0, 2) = _lookAt.x; (vm)(0, 3) = 0.0f;
		(vm)(1,0) = _right.y; (vm)(1, 1) = _up.y; (vm)(1, 2) = _lookAt.y; (vm)(1, 3) = 0.0f;
		(vm)(2,0) = _right.z; (vm)(2, 1) = _up.z; (vm)(2, 2) = _lookAt.z; (vm)(2, 3) = 0.0f;
		// left - right handed coordinate switch
		//(vm)(2,0) = -_right.z; (vm)(2, 1) = -_up.z; (vm)(2, 2) = -_lookAt.z; (vm)(2, 3) = 0.0f;
		(vm)(3,0) = x;        (vm)(3, 1) = y;     (vm)(3, 2) = z;		  (vm)(3, 3) = 1.0f;

		//
		return &vm;
	}

	void Camera::update()
	{
		float delta = 0.01f * _eye.z;
		//
		if (::GetAsyncKeyState('W') & 0x8000f)
		{
			walk(4.0f * delta);
		}
		if (::GetAsyncKeyState('S') & 0x8000f)
		{
			walk(-4.0f * delta);
		}
		if (::GetAsyncKeyState('A') & 0x8000f)
		{
			strafe(-4.0f * delta);
		}
		if (::GetAsyncKeyState('D') & 0x8000f)
		{
			strafe(4.0f * delta);
		}
		if (::GetAsyncKeyState('R') & 0x8000f)
		{
			fly(4.0f * delta);
		}
		if (::GetAsyncKeyState('F') & 0x8000f)
		{
			fly(-4.0f * delta);
		}
// 		if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
// 		{
// 			yaw(-4.0f * delta);
// 		}
	}

	void Camera::reset()
	{
		//
		_eye	= D3DXVECTOR3(0.0f, 0.0f, -100.0f);
		_right	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		_up		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//	_lookAt is a vector, indicating which direction the eye ray points
		_lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	Vec3 Camera::getEye()
	{
		return Vec3(_eye.x, _eye.y, _eye.z);
	}

	Vec3 Camera::getEyeSpaceX()
	{
		D3DXVec3Normalize(&_right, &_right);
		return Vec3(_right.x, _right.y, _right.z);
	}

	Vec3 Camera::getEyeSpaceY()
	{
		D3DXVec3Normalize(&_up, &_up);
		return Vec3(_up.x, _up.y, _up.z);
	}

	Vec3 Camera::getEyeSpaceZ()
	{
		D3DXVec3Normalize(&_lookAt, &_lookAt);
		return Vec3(_lookAt.x, _lookAt.y, _lookAt.z);
	}
}