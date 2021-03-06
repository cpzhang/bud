#pragma once
#include "IPrimitive.h"
#include "render/Euclid.h"
#include "AABBEntity.h"
class Geometry: public IPrimitive
{
public:
	Geometry();
	~Geometry();
	void setMaterial(const std::string& fileName);
	void setSkeletonMaterial(const std::string& fileName, Euclid::eMaterialType mt);
	void setSkin(const std::string& fileName);
	Euclid::IMaterial* getMaterial();
	void setMesh(const std::string& fileName);
	void setSkeleton(const std::string& fileName);
	virtual void update(u32 delta);
	virtual void decode(const std::string& fileName);
	void _renderMesh();
	void _renderAABB();
	virtual void render();
	virtual void renderSkeleton();
	virtual void destroy();
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix );
	void setBoneMapping(const std::string& fileName);
	void setAnimation(const std::string& fileName);
	virtual AniMapIterator getAniMapIterator(void) const;
	virtual void setAnimationName(const std::string& fileName);
	virtual void stopAnimation(){mStopAimation = true;};
	virtual void playAnimation(){mStopAimation = false;};
	virtual void setSpeed(float speed);
private:
	void _clear();
public:
	std::string mName;
	std::string mAnimationName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
	Euclid::IMaterial* mMaterialPW;
	Euclid::Skeleton* mSkeleton;
	Euclid::IMaterial* mSkeletonMaterial;
	Euclid::Skin* mSkin;
	AniMap mAnimations;
	Euclid::AnimationTime mAniTime;
	bool mHardwareSkin;
	Mat4 mUVMatrix; 
	bool mStopAimation;
	static float m_speed;
};
