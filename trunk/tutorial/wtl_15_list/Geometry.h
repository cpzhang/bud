#pragma once
#include "IPrimitive.h"
#include "render/Euclid.h"
class Geometry: public IPrimitive
{
public:
	Geometry();
	~Geometry();
	void setMaterial(const std::string& fileName, Euclid::eMaterialType mt);
	void setSkeletonMaterial(const std::string& fileName, Euclid::eMaterialType mt);
	void setSkin(const std::string& fileName);
	Euclid::IMaterial* getMaterial();
	void setMesh(const std::string& fileName);
	void setSkeleton(const std::string& fileName);
	virtual void update(u32 delta);
	virtual void decode(const std::string& fileName);
	void _renderMesh();
	virtual void render();
	virtual void renderSkeleton();
	virtual void destroy();
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix );
private:
	void _clear();
private:
	std::string mName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
	Euclid::Skeleton* mSkeleton;
	Euclid::IMaterial* mSkeletonMaterial;
	Euclid::Skin* mSkin;
};
