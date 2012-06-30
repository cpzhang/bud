#pragma once
#include "Material.h"
#include "MaterialSet.h"

typedef struct uvsettag
{
	short size;					// 坐标数（1－3）
} uvset;

typedef struct texcoordstag
{
	double u, v, w;				// 纹理坐标	
} texcoords;

typedef struct vbatag
{
	double weight;	//权重
	int jointIdx;	//相关得节点索引
} vba;

typedef struct vertextag
{
	double x, y, z;						//位置
	MVector n;							//法线
	float r,g,b,a;						//颜色
	std::vector<texcoords> texcoords;	//纹理坐标
	std::vector<vba> vbas;				//骨骼分配
} vertex;


typedef struct vertexInfotag
{
	int pointIdx;				// 点列表索引
	int normalIdx;				// 法线列表索引
	float r,g,b,a;				// 颜色
	std::vector<float> u;		// 纹理坐标U
	std::vector<float> v;		// 纹理坐标V
	std::vector<float> vba;		// 顶点骨骼分配
	std::vector<int> jointIds;	// 影响的节点索引
	int next;					// 同一位置顶点的索引
} vertexInfo;


typedef struct facetag
{
	long v[3];		// 顶点索引
} face;


typedef std::vector<face> faceArray;

class SubMesh
{
public:
	SubMesh(void);
	virtual ~SubMesh(void);

	void clear();
	MStatus loadMaterial(MObject& shader,MStringArray& uvsets);
	MStatus loadMaterialImp(MObject& shader,MStringArray& uvsets);
	MStatus load(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points,
		MFloatVectorArray& normals,bool opposite = false); 
	MStatus load_(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points,
		MFloatVectorArray& normals,bool opposite = false); 
	MStatus load__(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points,
		MFloatVectorArray& normals,bool opposite = false); 
public:
	//internal members
	MString m_name;
	bool	m_bAnimated;
	std::vector<material*> m_pMaterials;
	std::vector<vertex> m_vertices;
	std::vector<face> m_faces;
};
