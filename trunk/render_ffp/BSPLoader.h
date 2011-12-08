//==========================================================================
/**
* @file			: BSPLoader.h
* @author		: cpzhang <chengpengzhang@gmail.com>
* created		: 2010-5-20   10:12
* purpose		: 
* references	: http://www.mralligator.com/q3/#Visdata	 
				  http://www.mralligator.com/q3/trimesh/
*				  http://www.heppler.com/shader/
				  http://graphics.cs.brown.edu/games/quake/quake3.html
*/
//==========================================================================

#ifndef __BSPLoader_h__
#define __BSPLoader_h__

#include "Common.h"

namespace Euclid
{
	class BSPLoader
	{
	public:
		struct sHeader 
		{
			// BSP file identifier, This should always be 'IBSP'
			char indent[4];
			// BSP file version, 0x2e for the BSP files distributed with Quake 3.
			int version;
		};

		struct sLump
		{
			unsigned int offset;
			unsigned int length;
		};

		enum eLumps
		{
			kEntities		= 0,	// Stores player/object positions, etc...
			kShaders		= 1,	// Stores texture information
			kPlanes			= 2,	// Stores the splitting planes
			kNodes			= 3,	// Stores the BSP nodes
			kLeafs			= 4,	// Stores the leafs of the nodes
			kLeafFaces		= 5,	// Stores the leaf's indices into the faces
			kLeafBrushes	= 6,	// Stores the leaf's indices into the brushes
			kModels			= 7,	// Stores the info of world models
			kBrushes		= 8,	// Stores the brushes info (for collision)
			kBrushSides		= 9,	// Stores the brush surfaces info
			kVertices		= 10,	// Stores the level vertices
			kMeshVerts		= 11,	// Stores the model vertices offsets
			kFogs			= 12,	// Stores the shader files (blending, anims..)
			kFaces			= 13,	// Stores the faces for the level
			kLightmaps		= 14,	// Stores the lightmaps for the level
			kLightGrid		= 15,	// Stores extra world lighting information
			kVisData		= 16,	// Stores PVS and cluster info (visibility)
			kLightArray		= 17,	// RBSP
			kMaxLumps				// A constant to store the number of lumps
		};

		enum eToken
		{
			Q3_TOKEN_UNRESOLVED	= 0,
			Q3_TOKEN_EOF		= 1,
			Q3_TOKEN_START_LIST,
			Q3_TOKEN_END_LIST,
			Q3_TOKEN_ENTITY,
			Q3_TOKEN_TOKEN,
			Q3_TOKEN_EOL,
			Q3_TOKEN_COMMENT,
			Q3_TOKEN_MATH_DIVIDE,
			Q3_TOKEN_MATH_ADD,
			Q3_TOKEN_MATH_MULTIPY
		};

		struct sBSPTexture
		{
			char strName[64];			 // The name of the texture w/o the extension
			unsigned int flags;          // The surface flags (unknown)
			unsigned int contents;       // The content flags (unknown)
		};

		struct sBSPModel
		{
			float min[3];			  // The min position for the bounding box
			float max[3];             // The max position for the bounding box.
			int	  faceIndex;          // The first face index in the model
			int	  numOfFaces;         // The number of faces in the model
			int	  brushIndex;         // The first brush index in the model
			int	  numOfBrushes;       // The number brushes for the model
		};

		struct sBSPVertex
		{
			float vPosition[3];               // (x, y, z) position.
			float vTextureCoord[2];           // (u, v) texture coordinate
			float vLightmapCoord[2];          // (u, v) lightmap coordinate
			float vNormal[3];				  // (x, y, z) normal vector
			unsigned char color[4];           // RGBA color for the vertex
		};

		//	Vertex index offset, relative to first vertex of corresponding face.
typedef  int sBSPMeshVertices;

		//	The effects lump stores references to volumetric shaders (typically fog)
		//	which affect the rendering of a particular group of faces. 
		//	There are a total of length / sizeof(effect) records in the lump, where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPFog
		{
			char shader[64];		// The name of the shader file
			int  brushIndex;		// The brush index for this shader
			int	 visibleSide;	    // the brush side that ray tests need to clip against (-1 == none)
		};

		struct sBSPFace
		{
			enum eType
			{
				eType_Polygon = 1,
				eType_Patch,
				eType_Mesh,
				eType_Billboard
			};

			int textureID;        // The index into the texture array
			int fogNum;           // The index for the effects (or -1)
			int type;             // 1=polygon, 2=patch, 3=mesh, 4=billboard
			int vertexIndex;      // The index into this face's first vertex
			int numOfVerts;       // The number of vertices for this face
			int meshVertIndex;    // The index into the first meshvertex
			int numMeshVerts;     // The number of mesh vertices
			int lightmapID;       // The texture index for the lightmap
			int lMapCorner[2];    // The face's lightmap corner in the image
			int lMapSize[2];      // The size of the lightmap section
			float lMapPos[3];     // The 3D origin of lightmap.
			float lMapBitsets[2][3]; // The 3D space for s and t unit vectors.
			float vNormal[3];     // The face normal.
			int size[2];          // The bezier patch dimensions.
		};

		//	The lightmaps lump stores the light map textures used make surface lighting look more realistic. 
		//	There are a total of length / sizeof(lightmap) records in the lump, 
		//	where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPLightmap
		{
			unsigned char imageBits[128][128][3];   // The RGB data in a 128x128 image
		};
		

		//	The planes lump stores a generic set of planes that are in turn referenced by nodes and brushsides. There are a total of length / sizeof(plane) records in the lump, where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPPlane
		{
			float normal[3];	// Plane normal.
			float dist;			// Distance from origin to plane along normal.
		};

		//	The nodes lump stores all of the nodes in the map's BSP tree. 
		//	The BSP tree is used primarily as a spatial subdivision scheme, 
		//	dividing the world into convex regions called leafs. 
		//	The first node in the lump is the tree's root node. 
		//	There are a total of length / sizeof(node) records in the lump, 
		//	where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPNode
		{
			int plane;		//	Plane index.
			int children[2];//	Children indices. Negative numbers are leaf indices: -(leaf+1).
			int mins[3];	//	Integer bounding box min coord.
			int maxs[3];	//	Integer bounding box max coord.
		};

		//	The leafs lump stores the leaves of the map's BSP tree. Each leaf is a convex region that contains, among other things, a cluster index (for determining the other leafs potentially visible from within the leaf), a list of faces (for rendering), and a list of brushes (for collision detection). There are a total of length / sizeof(leaf) records in the lump, where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPLeaf 
		{
			int cluster;	//	Visdata cluster index. If cluster is negative, 
							//	the leaf is outside the map or otherwise invalid.
			int area;		//	Areaportal area.
			int mins[3];	//	Integer bounding box min coord.
			int maxs[3];	//	Integer bounding box max coord.
			int leafface;	//	First leafface for leaf.
			int n_leaffaces;//	Number of leaffaces for leaf.
			int leafbrush;	//	First leafbrush for leaf.
			int n_leafbrushes;	//	Number of leafbrushes for leaf.
		};

		//	The leaffaces lump stores lists of face indices, with one list per leaf. 
		//	There are a total of length / sizeof(leafface) records in the lump, 
		//	where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPLeafFace 
		{
			int index;	//	Face index.
		};

		//	The visdata lump stores bit vectors that provide cluster-to-cluster visibility information. There is exactly one visdata record, with a length equal to that specified in the lump directory.
		//	Cluster x is visible from cluster y if the (1 << y % 8) bit of vecs[x * sz_vecs + y / 8] is set.
		struct sBSPVisData
		{
			int n_vecs;		//	Number of vectors.
			int sz_vecs;	//	Size of each vector, in bytes.
			unsigned char* vecs;	//	Visibility data. One bit per cluster per vector.
		};

		//	The leafbrushes lump stores lists of brush indices, with one list per leaf.
		//	There are a total of length / sizeof(leafbrush) records in the lump, 
		//	where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPLeafBrush
		{
			int brush;	//	Brush index.
		};

		//	The brushsides lump stores descriptions of brush bounding surfaces. 
		//	There are a total of length / sizeof(brushsides) records in the lump, 
		//	where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPBrushSides
		{
			int plane;		//	Plane index.
			int texture;	//	Texture index.
		};

		//	The brushes lump stores a set of brushes, which are in turn used for collision detection.
		//	Each brush describes a convex volume as defined by its surrounding surfaces. 
		//	There are a total of length / sizeof(brushes) records in the lump, where length is the size of the lump itself, as specified in the lump directory.
		struct sBSPBrush
		{
			int brushside;		//	First brushside for brush.
			int n_brushsides;	//	Number of brushsides for brush.
			int texture;		// Texture index.
		};

		//
		struct sBSPLightGrid 
		{
			unsigned char	ambient[3];		//	Ambient color component. RGB.
			unsigned char	directional[3];	//	Directional color component. RGB.
			unsigned char	dir[2];			//	Direction to light. 0=phi, 1=theta.
		};

	public:
		BSPLoader();
		~BSPLoader();

	public:
		bool load(const std::string& name);

		void preRender();

		void postRender();

		void render2();
		void render();
		void renderNode(int index);
		void renderFace(int index);
		void renderLeaf(int index);
		//
	private:
		void _renderLight(int index);
		void _loadEntity();
		void _loadVisData();
		void _loadOtherStuff();

		ITexture* _getTexture(const std::string& name);
		ITexture* _getLightMapTexture(const int index);

	private:
		sHeader*					_header;
		sLump						_lumps[kMaxLumps];
		std::vector<sBSPMeshVertices>		_meshVertices;
		std::vector<sBSPTexture>	_textures;
		std::vector<sBSPModel>		_models;
		std::vector<sBSPVertex>		_vertices;
		std::vector<sBSPFog>		_fogs;
		std::vector<sBSPFace>		_faces;
		std::vector<sBSPLightmap>	_lightmaps;
		std::vector<sBSPPlane>		_planes;
		std::vector<sBSPNode>		_nodes;
		std::vector<sBSPLeaf>		_leafs;
		std::vector<sBSPLeafFace>	_leafFaces;
		std::vector<sBSPVisData>	_visData;
		std::vector<sBSPLeafBrush>	_leafBrushes;
		std::vector<sBSPBrushSides>	_brushSides;
		std::vector<sBSPBrush>		_brushes;
		std::vector<sBSPLightGrid>	_lightGrids;
		IFileRead*					_file;

		//
		IRenderSystem*				_renderSystem;
		//
		//ITexture*					_tex;
		typedef std::map<std::string, ITexture*>	TextureMap;
		typedef std::map<int, ITexture*>			LightMap;
		TextureMap					_texMap;
		LightMap					_lightMap;
	};
}

#endif // __BSPLoader_h__
 
