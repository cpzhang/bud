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
private:
	void _clear();
private:
	Euclid::IMaterial* mMaterialPW;
	Euler::AABB mAxisAlignedBoundingBox;
};
