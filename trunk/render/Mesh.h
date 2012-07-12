#ifndef __Mesh_h__
#define __Mesh_h__
#include "Common.h"
namespace Euclid
{
	typedef std::map<u8, int> BoneIDReferenceMap;
	class _EuclidExport_ Mesh
	{
	public:
		Mesh();
		~Mesh();
		bool create(const std::string& fileName);
		void createFromHeightMap(const std::string& fileName, int width, int height );
		bool createFromTerrain(Terrain* t);
		bool createFromMZ(size_t sub);
		void destroy();
		void clear();
		void render();
		int getFaceNumber();
		int getVertexNumber();
		bool save(const std::string& path);
		void saveSkin( const std::string& fileName );
		void saveSkeleton(const std::string& fileName);
		void saveBoneMapping(const std::string& fileName);
		void saveMaterial(const std::string& fileName);
		void loadBoneMapping(const std::string& fileName);
		AABB* getAABB();
		bool intersect(const Ray& r, Real& p);
		void geomipmapping(u32 lod);
	private:
		bool _initBuffer();
		void _initAABB();
		void _onLoaded();
	public: 
		struct sFace
		{
			sFace()
			{
				memset(this, 0, sizeof(*this));
			}
			sFace(u16 v0, u16 v1, u16 v2)
			{
				index[0] = v0;
				index[1] = v1;
				index[2] = v2;
			}
			u16 index[3];
		};
	public:
		typedef std::vector<sPositionTextureBoneWeightColorNormal> VertexPTBWCVec;
		VertexPTBWCVec	_vertices;
		typedef std::vector<sPositionTexture> VertexPTVec;
		VertexPTVec	_verticesPT;
		typedef std::vector<sFace> FaceVec;
		FaceVec	_faces;
		IBuffer* _vertexBuffer;
		IBuffer* _indexBuffer;
		std::string _name;
		static u8 msVersion;
		BoneIDReferenceMap _bones;
		eVertexDeclarationType mVDT;
		AABB mAABB;
		bool mIsTerrain;
		Real mTerrainHeight;
		Real mTerrainWidth;
	private:
		friend class Terrain;
		friend class MZ;
	};
}

#endif //__Mesh_h__