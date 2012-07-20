#pragma once
#include <algorithm>

class Sculptor
{
public:
	Sculptor()
	{
		_clear();
	}
	~Sculptor()
	{
		_clear();
	}
	void create()
	{
		//
		mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mMaterial->setEffect("shader\\Position.fx");
		//
		generateMesh(mRadius);
	}
	void destroy()
	{
		//
		if (mMaterial)
		{
			mMaterial->destroy();
			delete mMaterial;
		}
		_clear();
	}
	void render()
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
	void generateMesh(Real radius)
	{
		mRadius = radius;
		mVertices.clear();
		//构建平面圆
		for (Real alpha = 0.0f; alpha < Euler::TwoPI; alpha += 0.25f)
		{
			Vec3 p;
			p.x = mRadius * Euler::Basic::Cos(alpha);
			p.y = 0.0f;
			p.z = mRadius * Euler::Basic::Sin(alpha);
			mVertices.push_back(p);
		}
		mVertices.push_back(mVertices[0]);
		//射线拾取高度
	}
private:
	void _clear()
	{
		mRadius = 5.0f;
		mOrigion = Vec3::ZERO;
		mMaterial = NULL;
		mVertices.clear();
		mParaA = 0.01f;
	}
public:
	Real mRadius;
	Vec3 mOrigion;
	Real mParaA;
	Euclid::IMaterial* mMaterial;
	std::vector<Vec3> mVertices;
};