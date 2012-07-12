#pragma once
#include "math/Euler.h"
#include "render/Euclid.h"
class AABBEntity
{
public:
	AABBEntity();
	~AABBEntity();
	void update(Euler::AABB* ab);
	void render();
	void destroy();
	static void destroyMaterial();
	void setMatrix( const std::string& name, const Mat4& pMatrix );
	static void reset();
private:
	void _clear();
private:
	static Euclid::IMaterial* mMaterial;
	static Euler::AABB mAxisAlignedBoundingBox;
};
