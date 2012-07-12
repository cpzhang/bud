#include "AABBEntity.h"
#include "global.h"
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
	if (NULL == mMaterial)
	{
		mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mMaterial->setEffect("shader/aP.fx");
		mMaterial->setVertexDeclaration(Euclid::eVertexDeclarationType_Position);
		mMaterial->mFillMode = Euclid::eFillMode_WireFrame;
	}

	Euclid::Effect* fx = mMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mMaterial->apply();

	//
	Euler::AABB* ab = &mAxisAlignedBoundingBox;
	//
	Euclid::sPosition p[8];
	p[0].position = ab->_max;
	p[1].position = Vec3(ab->_max.x, ab->_max.y, ab->_min.z);
	p[2].position = Vec3(ab->_min.x, ab->_max.y, ab->_min.z);
	p[3].position = Vec3(ab->_min.x, ab->_max.y, ab->_max.z);

	p[4 + 0].position = Vec3(ab->_max.x, ab->_min.y, ab->_max.z);
	p[4 + 1].position = Vec3(ab->_max.x, ab->_min.y, ab->_min.z);
	p[4 + 2].position = ab->_min;
	p[4 + 3].position = Vec3(ab->_min.x, ab->_min.y, ab->_max.z);

	static u16 index[12 * 3] = 
	{
		//上
		0, 1, 2, 
		0, 2, 3,

		//下
		4 + 0, 4 + 1, 4 + 2,
		4 + 0, 4 + 2, 4 + 3,

		//前
		4 + 0, 0, 3,
		4 + 0, 3, 4 + 3,

		//后
		4 + 1, 1, 2,
		4 + 1, 2, 4 + 2,

		//左
		4 + 3, 3, 2,
		4 + 3, 2, 4 + 2,

		//右
		4 + 1, 1, 0,
		4 + 1, 0, 4 + 0,
	};
	//
	{
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0, 8, 12, &index[0], Euclid::eFormat_Index16, &p[0], sizeof(Euclid::sPosition));

			fx->endPass();
		}
		fx->end();
	}
}

void AABBEntity::_clear()
{
	//mMaterial = NULL;
	//mAxisAlignedBoundingBox.reset();
}

void AABBEntity::destroy()
{
	_clear();
}

void AABBEntity::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	if (mMaterial && mMaterial->getEffect())
	{
		mMaterial->getEffect()->setMatrix(name, pMatrix);
	}
}

void AABBEntity::destroyMaterial()
{
	if (mMaterial)
	{
		mMaterial->destroy();
		delete mMaterial;
	}
	mAxisAlignedBoundingBox.reset();
}

void AABBEntity::reset()
{
	mAxisAlignedBoundingBox.reset();
}

Euler::AABB AABBEntity::mAxisAlignedBoundingBox;

Euclid::IMaterial* AABBEntity::mMaterial(NULL);
