/********************************************************************
	author :	cpzhang
	created:	2012年-3月-23日   15:38
*********************************************************************/
#ifndef __Terrain_h__
#define __Terrain_h__
#include "Common.h"
#include "Mesh.h"
namespace Euclid
{
	class _EuclidExport_ Terrain
	{
	public:
		Terrain();
		~Terrain();

		bool create(const std::string& fileName);
		bool create();
		void destroy();
		void clear();
		void render();
		int getFaceNumber();
		int getVertexNumber();
	private:
		bool _initBuffer();
	public:
		const 
		//地形patch
		struct sSubMesh
		{
			u16 vstart;			// first vertex
			u16 vcount;			// num vertices
			u16 istart;			// first index
			u16 icount;			// num indices
			char name[32];
		};

	public:
		char _version;
		typedef std::vector<sPositionTexture> VertexVec;
		VertexVec	_vertices;
		typedef std::vector<sSubMesh> SubMeshVec;
		SubMeshVec	_submeshes;
		typedef std::vector<Mesh::sFace> FaceVec;
		FaceVec	_faces;
		IBuffer* _vertexBuffer;
		IBuffer* _indexBuffer;
	};
}

#endif //__Terrain_h__