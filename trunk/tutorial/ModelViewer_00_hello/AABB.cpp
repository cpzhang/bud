#include "AABBEntity.h"

AABBEntity::AABBEntity()
{
	_clear();
}

AABBEntity::~AABBEntity()
{

}

void AABBEntity::update( Euler::AABB* ab )
{
	if (NULL == ab)
	{
		return;
	}
	mAxisAlignedBoundingBox.updateMin(ab->_min);
	mAxisAlignedBoundingBox.updateMax(ab->_max);
}

void AABBEntity::render()
{

}

void AABBEntity::_clear()
{
	mAxisAlignedBoundingBox.reset();
}
