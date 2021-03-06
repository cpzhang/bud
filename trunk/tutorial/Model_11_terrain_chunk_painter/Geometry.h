#pragma once
#include "IPrimitive.h"
class Geometry: public IPrimitive
{
public:
	Geometry();
	~Geometry();
	IPrimitive_Derived

		void Geometry::setBoneMapping( const std::string& fileName );
	void Geometry::setSkin(const std::string& fileName);
	void Geometry::setAnimation( const std::string& fileName );

	void Geometry::setMesh(const std::string& fileName);
	void Geometry::setSkeleton(const std::string& fileName);

	void setMaterial(const std::string& fileName);

private:
	void _clear();
	void Geometry::_renderMesh();
	void Geometry::renderSkeleton();
public:
	std::string mName;
	std::string mMeshFile;
	Zen::Mesh* mMesh;
	Zen::IMaterial* mMaterial;
	Zen::Skeleton* mSkeleton;
	Zen::IMaterial* mSkeletonMaterial;
	Zen::Skin* mSkin;
	AniMap mAnimations;
	Zen::AnimationTime mAniTime;
	bool mHardwareSkin;
	Mat4 mUVMatrix; 
	bool mStopAimation;
	static float m_speed;
};