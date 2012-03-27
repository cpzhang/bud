#pragma	   once

typedef struct _modelversion 
{
	uint nVersion;
} MODELVERSION;

typedef struct _submesh_base
{
	ushort matId;			//����id
	ushort vstart;			// first vertex
	ushort vcount;			// num vertices
	ushort istart;			// first index
	ushort icount;			// num indices
	bool		animated;
	uchar		nameLen;
} SUBMESH_BASE;

typedef struct _submesh
{
	ushort matId;			//����id
	ushort vstart;			// first vertex
	ushort vcount;			// num vertices
	ushort istart;			// first index
	ushort icount;			// num indices
	bool		animated;
	char name[256];			// ��ģ������
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
	float color[4];	//�汾��19��ɫ�ŵ������
};

typedef struct _modelface
{
	short index[3];
} MODELFACE;

typedef struct _modelanimation
{
	std::string name;	// ��������
	uint startTime;
	uint endTime;
	uint nTimes;
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
	int parent;		// ���ڵ��id
	std::string name;
	tranMatrix tran;

	std::vector<ModelKeyframeTranslation> keyframesTranslation;
	std::vector<ModelKeyframeRotation> keyframesRotation;
	std::vector<ModelKeyframeScale> keyframesScale;
} MODELBONE;




