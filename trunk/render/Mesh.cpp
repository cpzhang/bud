#include "Mesh.h"
#include "RenderSystem.h"
#include "Buffer.h"
#include "BufferManager.h"
#include "MZ.h"
#include "base/DataChunk.h"
#include "tinyXML2/tinyxml2.h"
#include "VDTManager.h"
//
namespace Zen
{
	Mesh::Mesh()
	{
		clear();
	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::createFromMZ(size_t sub)
	{
		mVDT = eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
		//
		if (sub >= MZ::mSubmeshes.size())
		{
			return false;
		}
		
		_name = MZ::mSubmeshes[sub].name;

		//
		size_t vStart = MZ::mSubmeshes[sub].vstart;
		_vertices.resize(MZ::mSubmeshes[sub].vcount);
		std::ostringstream ss;
		ss<<"mesh "<<sub;
		Record(ss);
		Color c;
		for (size_t i = 0; i != _vertices.size(); ++i)
		{
			memcpy(&_vertices[i].position[0], MZ::mVertices[i + vStart].pos, sizeof(MZ::mVertices[i].pos));
			memcpy(&_vertices[i].normal[0], MZ::mVertices[i + vStart].normal, sizeof(MZ::mVertices[i].normal));
			memcpy(&_vertices[i].texcoord[0], MZ::mVertices[i+ vStart].texcoords, sizeof(MZ::mVertices[i].texcoords));
			memcpy(&c.r, MZ::mVertices[i+ vStart].color, sizeof(MZ::mVertices[i].color));
			_vertices[i].color_ARGB = c.getARGB();
			for(size_t k = 0; k != 4; ++k)
			{
				//_vertices[i].bones[k] = MZ::mVertices[i+ vStart].bones[k];
				u8 id = MZ::mVertices[i+ vStart].bones[k];
				//if (id < 255)
				{
					++_bones[id];
				}
			}
			memcpy(&_vertices[i].weights[0], MZ::mVertices[i+ vStart].weights, sizeof(MZ::mVertices[i].weights));
		}
		//骨骼重现分配
		for (size_t i = 0; i != _vertices.size(); ++i)
		{
			for(size_t k = 0; k != 4; ++k)
			{
				u8 id = MZ::mVertices[i+ vStart].bones[k];
				//
				_vertices[i].bones[k] = std::distance(_bones.begin(), _bones.find(id));
			}
		}
		//
		_faces.resize(MZ::mSubmeshes[sub].icount/3);
		size_t iStart = MZ::mSubmeshes[sub].istart / 3;
		for (size_t i = 0; i != _faces.size(); ++i)
		{
			_faces[i].index[0] = MZ::mFaces[i + iStart].index[0] - vStart;
			_faces[i].index[1] = MZ::mFaces[i + iStart].index[1] - vStart;
			_faces[i].index[2] = MZ::mFaces[i + iStart].index[2] - vStart;
		}

		//
		_initBuffer();
		_onLoaded();
		return true;
	}

	void Mesh::destroy()
	{
		if (_vertexBuffer)
		{
			_vertexBuffer->destroy();
			_vertexBuffer = NULL;
		}
		if (_indexBuffer)
		{
			_indexBuffer->destroy();
			_indexBuffer = NULL;
		}
		
		clear();
	}

	void Mesh::clear()
	{
		_vertexBuffer = 0;
		_indexBuffer = 0;
		_vertices.clear();
		_faces.clear();
		mVDT = eVertexDeclarationType_Null;
		mAABB.reset();
		mIsTerrain = false;
		mTerrainWidth = 0.0f;
		mTerrainHeight = 0.0f;
	}

	bool Mesh::_initBuffer()
	{
		sVDTNull* vdt = VDTManager::getInstancePtr()->mLayouts[mVDT];
		if (mVDT == eVertexDeclarationType_PositionTexture)
		{
			_vertexBuffer = BufferManager::getInstancePtr()->createVertexBuffer(_verticesPT.size() * vdt->size(), eUsage_Null, ePool_Manager);
			void* data = _vertexBuffer->lock(0, 0, Zen::eLock_Null);
			memcpy(data, &_verticesPT[0], _verticesPT.size() * vdt->size());
			_vertexBuffer->unLock();
		} 
		else if(mVDT == eVertexDeclarationType_PositionTextureBoneWeightColorNormal)
		{
			_vertexBuffer = BufferManager::getInstancePtr()->createVertexBuffer(_vertices.size() * vdt->size(), eUsage_Null, ePool_Manager);
			void* data = _vertexBuffer->lock(0, 0, Zen::eLock_Null);
			memcpy(data, &_vertices[0], _vertices.size() * vdt->size());
			_vertexBuffer->unLock();
		}
		else
		{
			return false;
		}
		
		//
		{
			_indexBuffer = BufferManager::getInstancePtr()->createIndexBuffer(_faces.size() * sizeof(sFace), eUsage_Null, eFormat_Index16, ePool_Manager);
			void* data = _indexBuffer->lock(0, 0, Zen::eLock_Null);
			memcpy(data, &_faces[0], _faces.size() * sizeof(sFace));
			_indexBuffer->unLock();
		}

		return true;
	}

	void Mesh::render()
	{
		sVDTNull* vdt = VDTManager::getInstancePtr()->mLayouts[mVDT];
		RenderSystem::getInstancePtr()->setStreamSource(0, _vertexBuffer, 0, vdt->size());
		RenderSystem::getInstancePtr()->setIndices(_indexBuffer);
		RenderSystem::getInstancePtr()->setVertexDeclaration(mVDT);
		if (mVDT == eVertexDeclarationType_PositionTexture)
		{
			RenderSystem::getInstancePtr()->drawIndexedPrimitive(ePrimitive_TriangleList, 0, 0, _verticesPT.size(), 0, _faces.size());
		} 
		else if(mVDT == eVertexDeclarationType_PositionTextureBoneWeightColorNormal)
		{
			RenderSystem::getInstancePtr()->drawIndexedPrimitive(ePrimitive_TriangleList, 0, 0, _vertices.size(), 0, _faces.size());
		}
		else
		{
			
		}
	}

	int Mesh::getFaceNumber()
	{
		return _faces.size();
	}

	int Mesh::getVertexNumber()
	{
		return _vertices.size();
	}

	bool Mesh::save( const std::string& path )
	{
		//============================================================================
		// 开始写入数据
		Zen::ChunkSet cs;

		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&msVersion, sizeof(msVersion));
		cs.endChunk();

		//============================================================================
		// 顶点
		sVDTNull* vdt = VDTManager::getInstancePtr()->mLayouts[mVDT];
		cs.beginChunk("MVTX");
		cs.write(&_vertices[0], vdt->size() * _vertices.size());
		cs.endChunk();

		//============================================================================
		// 索引
		cs.beginChunk("MFAC");
		cs.write(&_faces[0], sizeof(sFace) * _faces.size());
		cs.endChunk();

		//============================================================================
		// 保存文件，结束
		cs.save(path);
		
		return true;
	}

	void Mesh::saveSkeleton( const std::string& fileName )
	{
		//============================================================================
		// 开始写入数据
		Zen::ChunkSet cs;

		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&MZ::mVersion, sizeof(MZ::mVersion));
		cs.endChunk();

		//============================================================================
		// 顶点
		cs.beginChunk("MBON");
		u32 nBones = _bones.size();
		cs.write(&nBones, sizeof(nBones));
		nBones = MZ::mBones.size();
		for (size_t i = 0; i != nBones; ++i)
		{
			BoneIDReferenceMap::iterator it = _bones.find(i);
			if (it == _bones.end())
			{
				continue;
			}
			Bone& b = MZ::mBones[i];
			it = _bones.find(b.parent);
			if (it == _bones.end())
			{
				continue;
			}
			cs.write(&b.id, sizeof(b.id));

			u8 nameLength = b.name.size();
			cs.write(&nameLength, sizeof(nameLength));

			cs.write(b.name.c_str(), b.name.size());

			cs.write(&b.parent, sizeof(b.parent));

			cs.write(&b.initialMatrix, sizeof(b.initialMatrix));
		}

		cs.endChunk();

		//============================================================================
		// 保存文件，结束
		cs.save(fileName);
	}

	void Mesh::saveSkin( const std::string& fileName )
	{
		for (size_t i = 0; i != MZ::mSkins.size(); ++i)
		{
			std::string path = fileName + "/" + MZ::mAnimations[i].name + ".skin";
			Skin& s = MZ::mSkins[i];
			//============================================================================
			// 开始写入数据
			Zen::ChunkSet cs;

			//============================================================================
			// 版本号
			cs.beginChunk("MVER");	
			cs.write(&MZ::mVersion, sizeof(MZ::mVersion));
			cs.endChunk();

			//============================================================================
			// 顶点
			cs.beginChunk("MBON");
			u32 nBones = _bones.size();
			cs.write(&nBones, sizeof(nBones));
			for (size_t i = 0; i != nBones; ++i)
			{
				sBoneKFs& b = s.boneKFs[_bones[i]];
				//
				u32 num = b.translationKFs.numKeyFrames();
				cs.write(&num, sizeof(num));
				writeSequence(cs, b.translationKFs._keyFrames);
				//
				num = b.rotationKFs.numKeyFrames();
				cs.write(&num, sizeof(num));
				writeSequence(cs, b.rotationKFs._keyFrames);
				//
				num = b.scaleKFs.numKeyFrames();
				cs.write(&num, sizeof(num));
				writeSequence(cs, b.scaleKFs._keyFrames);
			}

			cs.endChunk();

			//============================================================================
			// 保存文件，结束
			cs.save(fileName);
		}
	}

	bool Mesh::create( const std::string& fileName )
	{
		std::ifstream f(fileName.c_str(), std::ios::binary);
		if (!f.good())
		{
			return false;
		}

		//	[Tag Size Data]
		int t;
		size_t s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(int));
			f.read((char*)&s, sizeof(size_t));

			if (s <= 0)
			{
				continue;
			}

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;

			switch (t)
			{
			case 'MVER':
				{
					f.read((char*)&msVersion, s);
				}
				break;

			case 'MVTX':
				{
					mVDT = eVertexDeclarationType_PositionTextureBoneWeightColorNormal;
					sVDTNull* vdt = VDTManager::getInstancePtr()->mLayouts[mVDT];
					_vertices.resize(s / vdt->size());
					f.read((char*)&_vertices[0], s);
				}
				break;
			case 'MFAC':
				{
					_faces.resize(s / sizeof(sFace));
					f.read((char*)&_faces[0], s);
				}
				break;
			}
		}

		_initBuffer();
		_onLoaded();
		return true;
	}

	void Mesh::saveBoneMapping( const std::string& fileName )
	{
		//============================================================================
		// 开始写入数据
		Zen::ChunkSet cs;

		//============================================================================
		// 版本号
		cs.beginChunk("MVER");	
		cs.write(&MZ::mVersion, sizeof(MZ::mVersion));
		cs.endChunk();

		//============================================================================
		// 顶点
		cs.beginChunk("MBON");
		u32 nBones = _bones.size();
		cs.write(&nBones, sizeof(nBones));
		
		for(BoneIDReferenceMap::iterator it = _bones.begin(); it != _bones.end(); ++it)
		{
			u8 id = std::distance(_bones.begin(), it);
			u8 boneID = it->first;
			//cs.write(&id, sizeof(u8));
			cs.write(&boneID, sizeof(u8));
		}

		cs.endChunk();

		//============================================================================
		// 保存文件，结束
		cs.save(fileName);
	}

	void Mesh::loadBoneMapping( const std::string& fileName )
	{
		std::ifstream f(fileName.c_str(), std::ios::binary);
		if (!f.good())
		{
			return;
		}

		//	[Tag Size Data]
		int t;
		size_t s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(int));
			f.read((char*)&s, sizeof(size_t));

			if (s <= 0)
			{
				continue;
			}

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;

			switch (t)
			{
			case 'MVER':
				{
					f.read((char*)&msVersion, s);
				}
				break;

			case 'MBON':
				{
					u32 nBones;
					f.read((char*)&nBones, sizeof(u32));
					//
					for (size_t i = 0; i != nBones; ++i)
					{
						u8 id;
						f.read((char*)&id, sizeof(u8));
						++_bones[id];
					}
				}
				break;
			}
		}
	}

	void Mesh::saveMaterial( const std::string& fileName )
	{
		tinyxml2::XMLDocument doc;
		// 
		tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
		doc.LinkEndChild(dec);
		//
		tinyxml2::XMLElement* ele = doc.NewElement("material");
		doc.LinkEndChild(ele);
		//
		doc.SaveFile(fileName.c_str());
	}

	bool Mesh::createFromTerrain( Terrain* t )
	{
		mVDT = eVertexDeclarationType_PositionTexture;
		size_t sz = VDTManager::getInstancePtr()->mLayouts[mVDT]->size();
		//
		_verticesPT.resize(t->_vertices.size());
		memcpy(&_verticesPT[0], &t->_vertices[0], _verticesPT.size() * sz);
		//
		_faces.resize(t->_faces.size());
		memcpy(&_faces[0], &t->_faces[0], _faces.size() * sizeof(sFace));
		//
		_initBuffer();
		_onLoaded();
		return true;
	}

	void Mesh::_onLoaded()
	{
		_initAABB();
	}

	void Mesh::_initAABB()
	{
		if (mVDT == eVertexDeclarationType_PositionTexture)
		{
			for (size_t i = 0; i != _verticesPT.size(); ++i)
			{
				mAABB.updateMax(_verticesPT[i].position);
				mAABB.updateMin(_verticesPT[i].position);
			}
		} 
		else if(mVDT == eVertexDeclarationType_PositionTextureBoneWeightColorNormal)
		{
			for (size_t i = 0; i != _vertices.size(); ++i)
			{
				mAABB.updateMax(_vertices[i].position);
				mAABB.updateMin(_vertices[i].position);
			}
		}
		else
		{
			mAABB.reset();
		}
	}

	AABB* Mesh::getAABB()
	{
		return &mAABB;
	}

	void Mesh::createFromHeightMap( const std::string& fileName, int width, int height )
	{
		//
		std::ifstream f(fileName.c_str(), std::ios::binary);
		if (!f.good())
		{
			return;
		}
		int length = 0;
		f.seekg(0, std::ios::end);
		length = f.tellg();
		if (length < width * height)
		{
			return;
		}
		
		f.seekg(0, std::ios::beg);
		u8* buffer = new u8[length];
		f.read((char*)buffer, length);
		//
		int half_width = width / 2;
		int half_height = height / 2;
		for (int i = 0; i != height; ++i)
		for (int j = 0; j != width; ++j)
		{
			Zen::sPositionTexture p;
			static float sfScale = 50.0f;
			static float sfHeightScale = 5.0f;
			p.position.x = (j - half_width) * sfScale;
			p.position.z = (i - half_height) * sfScale;
			p.position.y = (buffer[i + j * height] - 128) * sfHeightScale;
			p.texcoord.x = (float)i / (float)(height - 1);
			p.texcoord.y = (float)j / (float)(width - 1);
			_verticesPT.push_back(p);
		}
				
		//
		for (int i = 0; i != height - 1; ++i)
		for (int j = 0; j != width - 1; ++j)
		{
			u16 baseIndex = j + i * height;
			_faces.push_back(sFace(baseIndex, baseIndex + width, baseIndex + 1));
			_faces.push_back(sFace(baseIndex + width, baseIndex + 1 + width, baseIndex + 1));
		}
		delete[] buffer;
		buffer = NULL;
		f.close();
		mVDT = eVertexDeclarationType_PositionTexture;
		mIsTerrain = true;
		mTerrainHeight = height;
		mTerrainWidth = width;
		//
		_initBuffer();
		_onLoaded();
	}

	// a + a*a + a*a*a + ...
	float getSeriesSum(float a, int b)
	{
		if(b <= 1)
			return a;
		return Zen::Basic::power(a, b) + getSeriesSum(a, b - 1);
	}

	void Mesh::geomipmapping( u32 lod )
	{
		if (!mIsTerrain)
		{
			return;
		}
		// 更新三角形索引
		_faces.clear();
		int offset = 1<<lod;//Zen::Basic::power(2, lod);
		if (offset >= mTerrainWidth)
		{
			return;
		}
		for (int i = 0; i < mTerrainHeight - 1;i += offset)
		{
			for (int j = 0; j < mTerrainWidth - 1; j += offset)
			{
				u16 baseIndex = j + i * mTerrainWidth;
				//
				_faces.push_back(sFace(baseIndex, baseIndex + mTerrainWidth * offset, baseIndex + offset));
				_faces.push_back(sFace(baseIndex + mTerrainWidth * offset, baseIndex + offset + mTerrainWidth * offset, baseIndex + offset));
			}
		}

		// 更新IB
		{
			void* data = _indexBuffer->lock(0, 0, Zen::eLock_Null);
			memcpy(data, &_faces[0], _faces.size() * sizeof(sFace));
			_indexBuffer->unLock();
		}
	}

	bool isInsideTriangle(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p)
	{
		if (p == a || p == b || p == c)
		{
			return true;
		}
		Vec3 n0 = (p - a).crossProduct(b - a);
		Vec3 n1 = (p - b).crossProduct(c - b);
		Vec3 n2 = (p - c).crossProduct(a - c);
		Vec3 n = (b - a).crossProduct(c - a);
		bool s0 = n0.dotProduct(n) > 0;
		bool s1 = n1.dotProduct(n) > 0;
		bool s2 = n2.dotProduct(n) > 0;
		if (s0 == s1 && s1 == s2)
		{
			return true;
		}

		return false;
	}
	//耗时，需再思考算法
	// local space
	bool Mesh::intersect( const Ray& r, Real& p )
	{
		//简单起见，先处理地形
		if (!mIsTerrain)
		{
			return false;
		}
		std::vector<Real> ts;
		for (size_t i = 0; i != _faces.size(); ++i)
		{
			sFace& f = _faces[i];
			Vec3 p0 = _verticesPT[f.index[0]].position;
			Vec3 p1 = _verticesPT[f.index[1]].position;
			Vec3 p2 = _verticesPT[f.index[2]].position;
			// step 0, ray / plane intersect
			Vec3 n = (p1 - p0).crossProduct(p2 - p0);
			n.normalise();
			Vec3 s = r._origin - p1;
			Real a = n.dotProduct(r._direction);
			if (Zen::IsRealEqual(a, 0.0f))
			{
				continue;
			}
			Real b = n.dotProduct(s);
			Real t = - b / a;
			// step 1, check if the intersectin point is inside the triangle
			Vec3 q = r.getPoint(t);	
			if (Zen::isInsideTriangle(p0, p1, p2, q))
			{
				ts.push_back(t);
			}
		}
		// return the minimum one
		if (ts.size())
		{
			std::sort(ts.begin(), ts.end());
			p = ts[0];
		}
		return ts.size();
	}

	u8 Mesh::msVersion(0);
}
