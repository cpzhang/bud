//==========================================================================
/**
* @file	  : camera.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-9   15:17
* purpose : 
*/
//==========================================================================

#ifndef __camera_h__
#define __camera_h__

//
#include "stdafx.h"
#include "Common.h"

//
namespace Euclid
{
	//
	enum CameraType
	{
		Camera_LandObject,
		Camera_AirCraft
	};

	//
	class _EuclidExport_ Camera
	{
	public:
		Camera();
		Camera(CameraType ct);
		~Camera();
	public:
		// six camera operations
		//
		void strafe(float units);
		//
		void fly(float units);
		//
		void walk(float units);
		//
		void pitch(float angle);
		//
		void yaw(float angle);
		//
		void roll(float angle);


		//
		void setPosition(float x, float y, float z);
		Vec3 getEye();
		Vec3 getEyeSpaceX();
		Vec3 getEyeSpaceY();
		Vec3 getEyeSpaceZ();
		//
		void lookAt(float x, float y, float z);
		//
		D3DXMATRIX* getViewMatrix(D3DXMATRIX& vm);

		//
		void update();

		//
		void reset();

		//
	private:
		CameraType		_cameraType;
		// eye position
		D3DXVECTOR3		_eye;
		
		// the below three vectors compose another vector space basis
		
		// z
		D3DXVECTOR3		_lookAt;
		
		// x
		D3DXVECTOR3		_right;
		
		// y
		D3DXVECTOR3		_up;
	};
}

#endif // __camera_h__
 
