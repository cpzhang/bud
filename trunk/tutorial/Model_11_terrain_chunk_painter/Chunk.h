#pragma once
#include <algorithm>
#include "Sculptor.h"
class Layer
{
public:
	std::string mTexture;
	std::string mAlphaMap;
};
enum eChunkTextureLayer
{
	eChunkTextureLayer_0,
	eChunkTextureLayer_1,
	eChunkTextureLayer_2,
	eChunkTextureLayer_3,
	eChunkTextureLayer_Size = 4,
};
class Chunk
{
public:
	Chunk();
	~Chunk();
	void render();
	//根据最大LOD，生成chunk几何信息
	void generateMesh(size_t lod);
private:
	void _clear();
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
	Chunks();
	~Chunks();
	void open(const std::string& fileName);
	void save(const std::string& fileName);
	void create(size_t xNum, size_t yNum, size_t lod);
	Chunk* getChunk(size_t x, size_t y);
	Zen::IMaterial* getMaterial();
	void destroy();
	
	//地形圆掠过，高度变化
	void raise(Sculptor* s, bool heigher);
	Real getHeight(Real x, Real z);
	void render();
private:
	void _clear();
public:
	size_t mXNum;
	size_t mYNum;
	ChunkVec mChunks;
	Vec3 mPosition;
	Zen::IMaterial* mMaterial;
	static const float scChunkSize;
};