#pragma	   once
#include "SkeletonDefine.h"
typedef struct _modelversion 
{
	unsigned int nVersion;
} MODELVERSION;

//名字长度
const uchar gNameLength = 32;
//地形patch
struct sSubMesh
{
	unsigned short vstart;			// first vertex
	unsigned short vcount;			// num vertices
	unsigned short istart;			// first index
	unsigned short icount;			// num indices
	char		name[gNameLength];
};
//地形顶点
struct sVertex
{
	float pos[3];
	float texcoords[2];
};
typedef struct _submesh_base
{
	unsigned short matId;			//材质id
	unsigned short vstart;			// first vertex
	unsigned short vcount;			// num vertices
	unsigned short istart;			// first index
	unsigned short icount;			// num indices
	bool		animated;
	uchar		nameLen;
} SUBMESH_BASE;

typedef struct _submesh
{
	unsigned short matId;			//材质id
	unsigned short vstart;			// first vertex
	unsigned short vcount;			// num vertices
	unsigned short istart;			// first index
	unsigned short icount;			// num indices
	bool		animated;
	char name[256];			// 子模型名称
} SUBMESH;

typedef struct _modelvertex
{
	float pos[3];
	float normal[3];
	float color[4];
	float weights[4];
	uchar bones[4];
	float texcoords[2];
} MODELVERTEX;

typedef struct _modelvertex19Base
{
	float pos[3];
	float normal[3];
	float weights[4];
	uchar bones[4];
	float texcoords[2];
} MODELVERTEX_19BASE;

struct MODELVERTEX_19 : public MODELVERTEX_19BASE
{
	float color[4];	//版本号19颜色放到最后了
};

typedef struct _modelface
{
	short index[3];
} MODELFACE;

typedef struct _modelanimation
{
	std::string name;	// 动画名称
	unsigned int startTime;
	unsigned int endTime;
	unsigned int nTimes;
} MODELANIMATION;

struct ModelKeyframeTranslation
{
	int time;
	float v[3];
};

struct ModelKeyframeRotation
{
	int time;
	float q[4];
};

struct ModelKeyframeScale
{
	int time;
	float v[3];
};

typedef struct _modelbone
{
	int id;	
	int parent;		// 父节点的id
	std::string name;
	tranMatrix tran;

	std::vector<ModelKeyframeTranslation> keyframesTranslation;
	std::vector<ModelKeyframeRotation> keyframesRotation;
	std::vector<ModelKeyframeScale> keyframesScale;
} MODELBONE;




