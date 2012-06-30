#pragma once
#include "Material.h"
#include "MaterialSet.h"

typedef struct uvsettag
{
	short size;					// ��������1��3��
} uvset;

typedef struct texcoordstag
{
	double u, v, w;				// ��������	
} texcoords;

typedef struct vbatag
{
	double weight;	//Ȩ��
	int jointIdx;	//��صýڵ�����
} vba;

typedef struct vertextag
{
	double x, y, z;						//λ��
	MVector n;							//����
	float r,g,b,a;						//��ɫ
	std::vector<texcoords> texcoords;	//��������
	std::vector<vba> vbas;				//��������
} vertex;


typedef struct vertexInfotag
{
	int pointIdx;				// ���б�����
	int normalIdx;				// �����б�����
	float r,g,b,a;				// ��ɫ
	std::vector<float> u;		// ��������U
	std::vector<float> v;		// ��������V
	std::vector<float> vba;		// �����������
	std::vector<int> jointIds;	// Ӱ��Ľڵ�����
	int next;					// ͬһλ�ö��������
} vertexInfo;


typedef struct facetag
{
	long v[3];		// ��������
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
