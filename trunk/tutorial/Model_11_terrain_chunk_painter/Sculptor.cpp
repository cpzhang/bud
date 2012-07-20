#include "Sculptor.h"
#include "renderImp.h"
#include "Chunk.h"
bool isInsideCircle(Real x, Real y, Real ox, Real oy, Real radius)
{
	if ((x - ox) * (x - ox) + (y - oy) * (y - oy) <= radius * radius)
	{
		return true;
	}
	return false;
}
Sculptor::Sculptor()
	{
		_clear();
	}
	Sculptor::~Sculptor()
	{
		_clear();
	}
	void Sculptor::create()
	{
		//
		mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mMaterial->setEffect("shader\\Position.fx");
		//
		generateMesh(mRadius, NULL);
	}
	bool Sculptor::isInside(const Vec3& p)
	{
		return isInsideCircle(p.x, p.z, mOrigion.x, mOrigion.z, mRadius);
	}
	void Sculptor::destroy()
	{
		//
		if (mMaterial)
		{
			mMaterial->destroy();
			delete mMaterial;
		}
		_clear();
	}
	void Sculptor::render()
	{
		//
		if (NULL == mMaterial)
		{
			return;
		}
		Euclid::Effect* fx = mMaterial->getEffect();
		if (NULL == fx)
		{
			return;
		}
	
		Mat4 m = Mat4::IDENTITY;
		m.makeTransform(mOrigion, Vec3::UNIT_SCALE, Quaternion::IDENTITY);
		fx->setMatrix("g_mWorld", m);
		{
			Vec4 p(1.0f, 0.0f, 0.0f, 0.0f);
			fx->setVector("gAmbient", &p);
		}

		mMaterial->apply();
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);
			if (i == 0)
			{
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_LineStrip, mVertices.size() - 1,&mVertices[0], sizeof(Vec3));
			}
			fx->endPass();
		}
		fx->end();
	}
	void Sculptor::generateMesh(Real radius, Chunks* cks)
	{
		mVertices.clear();
		//构建圆
		if (radius <= 0.0001f)
		{
			radius = 1.0f;
		}
		mRadius = radius;
		Real step = Euler::TwoPI/ radius;
		for (Real alpha = 0.0f; alpha < Euler::TwoPI; alpha += step)
		{
			Vec3 p;
			p.x = mRadius * Euler::Basic::Cos(alpha);
			p.z = mRadius * Euler::Basic::Sin(alpha);
			p.y = 0.0f;
			//射线拾取高度
			if (cks)
			{
				p.y = cks->getHeight(p.x, p.z);
			}
			mVertices.push_back(p);
		}
		mVertices.push_back(mVertices[0]);
	}
	void Sculptor::_clear()
	{
		mRadius = 5.0f;
		mOrigion = Vec3::ZERO;
		mMaterial = NULL;
		mVertices.clear();
		mParaA = 0.01f;
	}

	void Sculptor::move( const Vec3& p, Chunks* cks )
	{
		mOrigion = p;
		if (cks == NULL)
		{
			return;
		}
		//射线拾取高度
		for (size_t i = 0; i != mVertices.size(); ++i)
		{
			Vec3& p= mVertices[i];
			p.y = cks->getHeight(p.x, p.z);
		}
	}
