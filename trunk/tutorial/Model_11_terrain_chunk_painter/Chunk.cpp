#include "Chunk.h"
#include "renderImp.h"
Chunk::Chunk()
{
	_clear();
}
Chunk::~Chunk()
{
	_clear();
}
void Chunk::render()
{
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Zen::ePrimitive_TriangleList, 0,
		mVertices.size(), mIndices.size()/3, &mIndices[0], Zen::eFormat_Index16, &mVertices[0], sizeof(Vec3));
}
//根据最大LOD，生成chunk几何信息
void Chunk::generateMesh(size_t lod)
{
	mMaxLOD = lod;
	mLOD = lod;
	mNum = Zen::Basic::power(2, lod) + 1;
	float r = 1.0f / ((float)mNum - 1.0f);
	//
	for (int i = 0; i != mNum; ++i)
	for (int j = 0; j != mNum; ++j)
	{
		Vec3 p;
		p.x = (float)j * r;
		p.z = -(float)i * r;
		p.y = 0.0f;
		mVertices.push_back(p);
	}

	//
	for (int i = 0; i != mNum - 1; ++i)
	for (int j = 0; j != mNum - 1; ++j)
	{
		u16 baseIndex = j + i * mNum;
		mIndices.push_back(baseIndex);
		mIndices.push_back(baseIndex + mNum);
		mIndices.push_back(baseIndex + 1);

		mIndices.push_back(baseIndex + mNum);
		mIndices.push_back(baseIndex + 1 + mNum);
		mIndices.push_back(baseIndex + 1);
	}
}
void Chunk::_clear()
{
	mPostion = Vec2::ZERO;
	mNum = 1;
	mLOD = 0;
	mMaxLOD = 0;
	mVertices.clear();
	mIndices.clear();
}

Chunks::Chunks()
{
	_clear();
}
Chunks::~Chunks()
{
	_clear();
}
void Chunks::open(const std::string& fileName)
{

}
void Chunks::save(const std::string& fileName)
{

}
void Chunks::create(size_t xNum, size_t yNum, size_t lod)
{
	mXNum = xNum;
	mYNum = yNum;
	mChunks.resize(mXNum * mYNum);
	for (size_t i = 0; i != mChunks.size(); ++i)
	{
		mChunks[i] = new Chunk;
		mChunks[i]->mPostion.x = i % mXNum;
		mChunks[i]->mPostion.y = i / mXNum;
		mChunks[i]->generateMesh(lod);
	}
	//
	mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Zen::eMaterialType_Vertex);
	mMaterial->setEffect("shader\\Position.fx");
	mMaterial->mFillMode = Zen::eFillMode_Solid;
}
Chunk* Chunks::getChunk(size_t x, size_t y)
{
	return mChunks[x + y * mXNum];	
}
Zen::IMaterial* Chunks::getMaterial()
{
	return mMaterial;
}
void Chunks::destroy()
{
	for (size_t i = 0; i != mChunks.size(); ++i)
	{
		Chunk* c = mChunks[i];
		if (c)
		{
			delete c;
		}
		mChunks[i] = NULL;
	}
	//
	if (mMaterial)
	{
		mMaterial->destroy();
		delete mMaterial;
	}
	_clear();
}

//地形圆掠过，高度变化
void Chunks::raise(Sculptor* s, bool heigher)
{
	Real radiusSquare = s->mRadius * s->mRadius;
	//遍历顶点，筛选出地形圆覆盖的顶点
	for (size_t i = 0; i != mChunks.size(); ++i)
	{
		Chunk* c = mChunks[i];
		//先过滤大部分的chunk
		{
			//如何判断一个圆是否与一个正方形有交集？
			Vec3 leftLow = mPosition + Vec3(scChunkSize * c->mPostion.x, 0.0f, -scChunkSize * c->mPostion.y);
			Vec3 center = leftLow;
			center.x += 0.5f * scChunkSize;
			center.y = 0.0f;
			center.z -= 0.5f * scChunkSize;
			Vec3 delta = center - s->mOrigion;
			if (delta.length() >= s->mRadius + scChunkSize * 0.5f * 1.414f)
			{
				continue;
			}
		}
		Mat4 m;
		m.makeTransform(mPosition + Vec3(scChunkSize * c->mPostion.x, 0.0f, -scChunkSize * c->mPostion.y), Vec3(scChunkSize, 1.0f, scChunkSize), Quaternion::IDENTITY);
		for (size_t k = 0; k != c->mVertices.size(); ++k)
		{
			Vec3 p = c->mVertices[k];
			//世界坐标系
			p = m * p;
			//在xz平面上，判断顶点是否在圆内部
			Real deltaX = p.x - s->mOrigion.x;
			Real deltaZ = p.z - s->mOrigion.z;
			Real distanceSquare = deltaX * deltaX + deltaZ * deltaZ;
			if (distanceSquare <= radiusSquare)
			{
				Real delta = Zen::Basic::Sqrt(radiusSquare - distanceSquare);
				delta *= s->mParaA;
				
				//选中，处理高度变化
				if (heigher)
				{
					c->mVertices[k].y += delta;
				}
				else
				{
					c->mVertices[k].y -= delta;
				}
			}
		}
	}
}
void Chunks::render()
{
	//
	if (NULL == mMaterial)
	{
		return;
	}
	Zen::Effect* fx = mMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	for (size_t i = 0; i != mChunks.size(); ++i)
	{
		Chunk* c = mChunks[i];
		if (c)
		{
			Mat4 m;
			m.makeTransform(mPosition + Vec3(scChunkSize * c->mPostion.x, 0.0f, -scChunkSize * c->mPostion.y), Vec3(scChunkSize, 1.0f, scChunkSize), Quaternion::IDENTITY);
			fx->setMatrix("g_mWorld", m);
			{
				// 用颜色来区分邻居chunk
				Vec4 p;
				if ((int)c->mPostion.x % 2 == 0)
				{
					p.x = 0.0f;
				}
				else
				{
					p.x = 1.0f;
				}
				
				//
				if ((int)c->mPostion.y % 2 == 0)
				{
					p.z = 0.0f;
				}
				else
				{
					p.z = 1.0f;
				}
				//
				p.y = 2 - (p.x + p.z);
				p.y /= 2.0f;
				//
				p.w = 1.0f;
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
					c->render();
				}
				fx->endPass();
			}
			fx->end();
		}
	}
	
}
void Chunks::_clear()
{
	mXNum = 0;
	mYNum = 0;
	mChunks.clear();
	mPosition = Vec3::ZERO;
	mMaterial = NULL;
}

Real Chunks::getHeight( Real x, Real z )
{
	Real height = 0.0f;
	//遍历顶点，筛选出地形圆覆盖的顶点
	for (size_t i = 0; i != mChunks.size(); ++i)
	{
		Chunk* c = mChunks[i];
		//先过滤大部分的chunk
		{
			//如何判断一个点是否在一个正方形？
			Vec3 leftLow = mPosition + Vec3(scChunkSize * c->mPostion.x, 0.0f, -scChunkSize * c->mPostion.y);
			Real maxX = leftLow.x + scChunkSize;
			Real minX = leftLow.x;
			Real maxZ = leftLow.z;
			Real minZ = leftLow.z - scChunkSize;
			if (x < minX || x > maxX || z < minZ || z > maxZ)
			{
				continue;
			}
		}
		Mat4 m;
		m.makeTransform(mPosition + Vec3(scChunkSize * c->mPostion.x, 0.0f, -scChunkSize * c->mPostion.y), Vec3(scChunkSize, 1.0f, scChunkSize), Quaternion::IDENTITY);
		std::vector<Real> ts;
		Vec3 p = Vec3(x, 0.0f, z);
		Ray r;
		r._direction = Vec3::NEGATIVE_UNIT_Y;
		r._origin = Vec3(x, 100000.0f, z);
		for (size_t k = 0; k != c->mIndices.size(); k += 3)
		{
			Vec3 p0 = m * c->mVertices[c->mIndices[k]];
			Vec3 p1 = m * c->mVertices[c->mIndices[k + 1]];
			Vec3 p2 = m * c->mVertices[c->mIndices[k + 2]];
			std::pair<bool, Real> result = Zen::intersects(r, p0, p1, p2);
			if (result.first)
			{
				height = r.getPoint(result.second).y;
				return height;
			}
		}
	}
	return height;
}

const float Chunks::scChunkSize(100.0f);
