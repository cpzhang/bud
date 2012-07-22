/********************************************************************
**	file: 		Camera.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-5-25
**	purpose:	
*********************************************************************/
#ifndef __Camera_h__
#define __Camera_h__
#include "Common.h"

namespace Zen
{
#define New_PitchYaw
	class _ZenExport_ Camera
	{
	public:
		Camera();
		void  calcViewMatrix();
		void  calcProjectionMatrix();
		void  calcOrthoProjectionMatrix();
		const Mat4& getViewMatrix();
		const Mat4& getProjectionMatrix();
		const Mat4& getOrthoProjectionMatrix();
        /** Rolls the camera anticlockwise, around its local z axis.
        */
        void roll(const Radian& angle);

        /** Rotates the camera anticlockwise around it's local y axis.
        */
        void yaw(const Radian& angle);

        /** Pitches the camera up/down anticlockwise around it's local z axis.
        */
        void pitch(const Radian& angle);

        /** Rotate the camera around an arbitrary axis.
        */
        void rotate(const Vec3& axis, const Radian& angle);

        /** Rotate the camera around an arbitrary axis using a Quaternion.
        */
        void rotate(const Quaternion& q);
        /** Sets the camera's position.
        */
        void setPosition(const Vec3& vec);
		Vec3 getPosition();
		/** Moves the camera's position by the vector offset provided along world axes.
        */
        void move(const Vec3& vec);

        /** Moves the camera's position by the vector offset provided along it's own axes (relative to orientation).
        */
        void moveRelative(const Vec3& vec);
        /** Sets the camera's direction vector.
        */
		void setDirection(const Vec3& vec, const Vec3& up = Vec3::UNIT_Y);

        /** Points the camera at a location in worldspace.
            @remarks
                This is a helper method to automatically generate the
                direction vector for the camera, based on it's current position
                and the supplied look-at point.
            @param
                targetPoint A vector specifying the look at point.
        */
        void lookAt( const Vec3& targetPoint , const Vec3& up = Vec3::UNIT_Y);
        /** Sets the position of the near clipping plane.
            @remarks
                The position of the near clipping plane is the distance from the frustums position to the screen
                on which the world is projected. The near plane distance, combined with the field-of-view and the
                aspect ratio, determines the size of the viewport through which the world is viewed (in world
                co-ordinates). Note that this world viewport is different to a screen viewport, which has it's
                dimensions expressed in pixels. The frustums viewport should have the same aspect ratio as the
                screen viewport it renders into to avoid distortion.
            @param
                near The distance to the near clipping plane from the frustum in world coordinates.
         */
        virtual void setNearClipDistance(Real nearDist);


        /** Sets the distance to the far clipping plane.
            @remarks
                The view frustum is a pyramid created from the frustum position and the edges of the viewport.
                This method sets the distance for the far end of that pyramid. 
                Different applications need different values: e.g. a flight sim
                needs a much further far clipping plane than a first-person 
                shooter. An important point here is that the larger the ratio 
                between near and far clipping planes, the lower the accuracy of
                the Z-buffer used to depth-cue pixels. This is because the
                Z-range is limited to the size of the Z buffer (16 or 32-bit) 
                and the max values must be spread over the gap between near and
                far clip planes. As it happens, you can affect the accuracy far 
                more by altering the near distance rather than the far distance, 
                but keep this in mind.
            @param
                far The distance to the far clipping plane from the frustum in 
                world coordinates.If you specify 0, this means an infinite view
                distance which is useful especially when projecting shadows; but
                be careful not to use a near distance too close.
        */
        virtual void setFarClipDistance(Real farDist);

		Vec3 getDirection(void ) const;
		Vec3 getUp(void ) const;
		Vec3 getRight(void ) const;

		void update();
		void setControllerThirdPerson(CameraControllerThirdPerson* ctp);
		CameraControllerThirdPerson* getControllerThirdPerson();
		void setDirty();
		void makeQuaternion();
		Ray getCameraToViewportRay(float x,float y) const;
	private:
		bool			m_dirty;
		Vec3			m_vPosition;
		Quaternion		m_qtnOrientation;
		Radian	m_radFOVy;
		float	m_fNearDistance;
		float	m_fFarDistance;
		Mat4 m_mtxProjection;
		Mat4 m_mtxOrthoProjection;
		Mat4 m_mtxView;
		CameraControllerThirdPerson*	_ctp;
		Radian m_pitchAngle;
		Radian m_yawAngle;
	};
}


#endif // __Camera_h__