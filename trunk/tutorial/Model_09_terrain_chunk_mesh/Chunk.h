#pragma once
#include <algorithm>
class Layer
{
public:
	std::string mTexture;
	std::string mAlphaMap;
};
class Chunk
{
public:
	Chunk()
	{
		_clear();
	}
	~Chunk()
	{
		_clear();
	}
	void render()
	{
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0,
			mVertices.size(), mIndices.size()/3, &mIndices[0], Euclid::eFormat_Index16, &mVertices[0], sizeof(Vec3));
	}
	void generateMesh(size_t lod)
	{
		mMaxLOD = lod;
		mLOD = lod;
		mNum = Euler::Basic::power(2, lod) + 1;
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
private:
	void _clear()
	{
		mPostion = Vec2::ZERO;
		mNum = 1;
		mLOD = 0;
		mMaxLOD = 0;
		mVertices.clear();
		mIndices.clear();
	}
public:
	size_t mNum;
	Vec2 mPostion;
	size_t mLOD;
	size_t mMaxLOD;
	std::vector<Vec3> mVertices;
	std::vector<u16> mIndices;
};
typedef std::vector<Chunk*> ChunkVec;
class Chunks
{
public:
	Chunks()
	{
		_clear();
	}
	~Chunks()
	{
		_clear();
	}
	void open(const std::string& fileName)
	{

	}
	void save(const std::string& fileName)
	{

	}
	void create(size_t xNum, size_t yNum, size_t lod)
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
		mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mMaterial->setEffect("shader\\Position.fx");
		mMaterial->mFillMode = Euclid::eFillMode_WireFrame;
	}
	Chunk* getChunk(size_t x, size_t y)
	{
		return mChunks[x + y * mXNum];	
	}
	Euclid::IMaterial* getMaterial()
	{
		return mMaterial;
	}
	void destroy()
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
		static const float scChunkSize = 100.0f;
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
private:
	void _clear()
	{
		mXNum = 0;
		mYNum = 0;
		mChunks.clear();
		mPosition = Vec3::ZERO;
		mMaterial = NULL;
	}
public:
	size_t mXNum;
	size_t mYNum;
	ChunkVec mChunks;
	Vec3 mPosition;
	Euclid::IMaterial* mMaterial;
};