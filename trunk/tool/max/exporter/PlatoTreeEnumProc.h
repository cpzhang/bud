/********************************************************************
**	file: 		PlatoTreeEnumProc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-3-16
**	purpose:	
*********************************************************************/
#ifndef __PlatoTreeEnumProc_h__
#define __PlatoTreeEnumProc_h__

#include "PlatoCommon.h"
#include "SkeletonDefine.h"
#include "MHeader.h"
#include "material.h"

enum eType
{
	eType_Unknown,
	eType_Mesh,
	eType_Material,
	eType_Bone,
	eType_Biped,
	eType_Helper,
	eType_Particle,
	eType_Size
};

bool IsRealEqual(const float& a, const float& b);
struct sKeyframeTranslation
{
	int time;
	float v[3]; 
	sKeyframeTranslation()
	{

	}
	sKeyframeTranslation(const sKeyframeTranslation& rk)
	{
		time = rk.time;
		memcpy(v, rk.v, 3*sizeof(float));
	}
	inline bool operator == (const sKeyframeTranslation& rk) const
	{
		return (IsRealEqual(v[0], rk.v[0]) && IsRealEqual(v[1], rk.v[1]) && IsRealEqual(v[2], rk.v[2]));
	}
	inline bool isStill()
	{
		return (v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f);
	}

	inline sKeyframeTranslation linear(float r, const sKeyframeTranslation& rk) const
	{
		sKeyframeTranslation s;
		s.v[0] = r * v[0] + (1 - r) * rk.v[0];
		s.v[1] = r * v[1] + (1 - r) * rk.v[1];
		s.v[2] = r * v[2] + (1 - r) * rk.v[2];

		return s;
	}
};

struct sKeyframeScale
{
	int time;
	float v[3];
	sKeyframeScale()
	{

	}
	sKeyframeScale(const sKeyframeScale& rk)
	{
		time = rk.time;
		memcpy(v, rk.v, 3*sizeof(float));
	}
	inline bool operator == (const sKeyframeScale& rk) const
	{
		return (IsRealEqual(v[0], rk.v[0]) && IsRealEqual(v[1], rk.v[1]) && IsRealEqual(v[2], rk.v[2]));
	}
	inline bool isStill()
	{
		return (v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f);
	}
	inline sKeyframeScale linear(float r, const sKeyframeScale& rk) const
	{
		sKeyframeScale s;
		s.v[0] = r * v[0] + (1 - r) * rk.v[0];
		s.v[1] = r * v[1] + (1 - r) * rk.v[1];
		s.v[2] = r * v[2] + (1 - r) * rk.v[2];

		return s;
	}
};

struct sKeyframeRotation
{
	int time;
	float q[4];
	sKeyframeRotation()
	{

	}
	sKeyframeRotation(const sKeyframeRotation& rk)
	{
		time = rk.time;
		memcpy(q, rk.q, 4*sizeof(float));
	}
	inline bool operator == (const sKeyframeRotation& rk) const
	{
		return (IsRealEqual(q[0], rk.q[0]) && IsRealEqual(q[1], rk.q[1]) && IsRealEqual(q[2], rk.q[2]) && IsRealEqual(q[3], rk.q[3]));
	}
	inline bool isStill()
	{
		return (q[0] == 0.0f && q[1] == 0.0f && q[2] == 0.0f);
	}

	inline sKeyframeRotation linear(float r, const sKeyframeRotation& rk) const
	{
		sKeyframeRotation s;
		s.q[0] = r * q[0] + (1 - r) * rk.q[0];
		s.q[1] = r * q[1] + (1 - r) * rk.q[1];
		s.q[2] = r * q[2] + (1 - r) * rk.q[2];
		s.q[3] = r * q[3] + (1 - r) * rk.q[3];

		return s;
	}
};

template<class T> bool IsStill(T& v)
{
	if (v.empty())
	{
		return true;
	}

	T::iterator it = v.begin();
	for (; it != v.end(); ++it)
	{
		if (!it->isStill())
		{
			return false;
		}
	}

	return true;
}
struct sBone
{
	sBone()
	{
		id = -1;
		parentId = -1;
		referrenced = false;
		unReferrenced = false;
		oldId = -1;
	}
	std::string name;
	std::string parent;
	int id;
	int parentId;
	std::vector<int>	children;
	std::vector<sKeyframeTranslation> translationSet;
	std::vector<sKeyframeRotation> rotationSet;
	std::vector<sKeyframeScale> scaleSet;
	//
	Matrix3 mat;
	Matrix3 cmat;
	bool	referrenced;
	bool	unReferrenced;
	int oldId;
	bool isStill()
	{
		return (IsStill(translationSet) && IsStill(rotationSet) && IsStill(scaleSet));
	}
	void processStill()
	{
		if (IsStill(translationSet))
		{
			translationSet.clear();
		}
		if (IsStill(rotationSet))
		{
			rotationSet.clear();
		}
		if (IsStill(scaleSet))
		{
			scaleSet.clear();
		}
	}
	void sampleLinear()
	{
		SampleLinear(translationSet, 1);
		SampleLinear(rotationSet, 1);
		SampleLinear(scaleSet, 1);
	}
};

struct sParticle
{
	std::string bone;
	bool visible;
	float speed;
	float variation;
	float coneAngle;
	float gravity;
	float explosiveForce;
	float life;
	float lifeVar;		//生命值的随机值，比如1.0f，表示生命[life - 1.0f,life + 1.0f]
	float emissionRate;
	short initialNum;
	short limitNum;
	bool attachToEmitter;
	bool moveWithEmitter;
	bool forTheSword;
	float forTheSwordInitialAngle;
	bool wander;
	float wanderRadius;
	float wanderSpeed;
	float aspectRatio;
	float initialAngleBegin;
	float initialAngleEnd;
	float rotationSpeed;
	float rotationSpeedVar;
	float width;
	float length;
	float height;
	short blendMode;
	std::string textureFilename;
	short textureRows;
	short textureCols;
	short changeStyle;
	short changeInterval;
	float tailLength;
	float timeMiddle;
	float3 colorStart;
	float3 colorMiddle;
	float3 colorEnd;
	float3 alpha;
	float3 scale;
	float3 scaleVar;
	bool fixedSize;
	short3 headLifeSpan;
	short3 headDecay;
	short3 tailLifeSpan;
	short3 tailDecay;
	bool head;
	bool tail;
	bool unshaded;
	bool unfogged;
	bool blockByY0;

	std::vector<keyframeT<bool> > keyframesVisible;
	std::vector<keyframeT<float> > keyframesSpeed;
	std::vector<keyframeT<float> > keyframesVariation;
	std::vector<keyframeT<float> > keyframesConeAngle;
	std::vector<keyframeT<float> > keyframesGravity;
	std::vector<keyframeT<float> > keyframesExplosiveForce;
	std::vector<keyframeT<float> > keyframesEmissionRate;
	std::vector<keyframeT<float> > keyframesWidth;
	std::vector<keyframeT<float> > keyframesLength;
	std::vector<keyframeT<float> > keyframesHeight;	//版本号16加入

	void sampleLinear()
	{
		SampleLinear(keyframesVisible, 1);
		SampleLinear(keyframesSpeed, 1);
		SampleLinear(keyframesVariation, 1);
		SampleLinear(keyframesConeAngle, 1);
		SampleLinear(keyframesGravity, 1);
		SampleLinear(keyframesExplosiveForce, 1);
		SampleLinear(keyframesEmissionRate, 1);
		SampleLinear(keyframesWidth, 1);
		SampleLinear(keyframesLength, 1);
		SampleLinear(keyframesHeight, 1);
	}
	void processStillSequence()
	{
		if (CheckAllKeyFramesTheSame(keyframesVisible))
		{
			keyframesVisible.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesSpeed))
		{
			keyframesSpeed.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesVariation))
		{
			keyframesVariation.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesConeAngle))
		{
			keyframesConeAngle.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesGravity))
		{
			keyframesGravity.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesExplosiveForce))
		{
			keyframesExplosiveForce.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesEmissionRate))
		{
			keyframesEmissionRate.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesWidth))
		{
			keyframesWidth.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesLength))
		{
			keyframesLength.clear();
		}
		if (CheckAllKeyFramesTheSame(keyframesHeight))
		{
			keyframesHeight.clear();
		}
	}
};

class PlatoTreeEnumProc : public ITreeEnumProc
{
public:
	PlatoTreeEnumProc();
	virtual ~PlatoTreeEnumProc();

public:
	virtual int callback( INode *node );

	void write();
public:
	void setTime(TimeValue t);

	void addNode(INode* n);

	bool read();
	bool readTerrain();
	void readBone();
	bool readMesh();
	void readParticle();
	void writeBone();
	static bool IsUnReferrenced(const sBone& b);
private:
	void _clear();

	void _getKeyFrame(Control* tmc);

	bool _extractMaterial(INode* node, unsigned short& matID);

	void _extractMesh(INode* node);
	void _extractTerrain(INode* node);

	void _extractSkin(INode* node);

	void _extractBone(INode* node);

	void _extractBiped(INode* node);

	void _extractParticle(INode* node);

	bool _isMaterialExtracted(Mtl* m, unsigned short& matID);

	Modifier* _getPhysiqueModifier(INode *pNode);

	eType _extractType(INode* node);
	void _setBoneParent();
	std::string _getParticleBone(INode* node);
	bool _isBoneSelected();
private:
	TimeValue	_time;
	std::vector<SUBMESH>	_subMeshes;
	std::vector<material>	_materials;
	std::vector<MODELVERTEX_19>	_vertices;
	typedef std::pair<std::string, float> BoneIndexWeight;
	typedef std::vector<BoneIndexWeight> VertexBW;
	std::vector<VertexBW>	_verticesBones;
	std::vector<MODELFACE>			_faces;
	std::vector<Matrix3>			_matrices;
	ushort tIStart;
	ushort tVStart;
	std::vector<INode*>	_nodeSet;
	static std::vector<sBone>	_boneSet;
	IGameScene*		_gameScene;
	std::vector<sParticle> _particleSet;
	bool	_currentNodeIsRef;
public:
	static bool cmpBoneWeight(BoneIndexWeight left, BoneIndexWeight right)
	{
		return left.second > right.second;
	}
	static Point3 convert2Standard(const Point3& p)
	{
		Point3 sp;
		sp.x = p.x;
		sp.y = p.y;
		sp.z = p.z;

		return sp;
	}
	static Quat convert2Standard(const Quat& p)
	{
		Quat sp;
		sp.x = p.x;
		sp.y = p.y;
		sp.z = p.z;
		sp.w = p.w;

		return sp;
	}
	// 
	static Matrix3 convert2Standard(const Matrix3& p)
	{
		Matrix3 base(1);
		base.SetRow(0, Point3(1,  0,  0));
		base.SetRow(1, Point3(0,  0, -1));
		base.SetRow(2, Point3(0,  1,  0));
		return Inverse(base) * p * base;
	}
	static Matrix3 convert2LeftHandedCoordination(const Matrix3& p)
	{
		Matrix3 m;
		m.SetColumn(0, p.GetColumn(0));
		m.SetColumn(1, p.GetColumn(2));
		m.SetColumn(2, p.GetColumn(1));
		return m;
	}
	friend INT_PTR CALLBACK DeleteRedundantBonesDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void createTreeItem(HWND hDlg, sBone& b, HTREEITEM h);
};

class MemoryFileWriter
{
public:
	MemoryFileWriter()
	{
		_cursor = 0;
		_length = 0;
		_buffer = NULL;
	}
	~MemoryFileWriter()
	{
		if (_buffer)
		{
			delete _buffer;
			_buffer = NULL;
		}

		_cursor = 0;
		_length = 0;
	}
public:
	bool isEnd()
	{
		return _cursor >= _length;
	}

	bool isOverFlow(size_t s)
	{
		return s + _cursor >= _length;
	}

	bool reallocate(size_t s)
	{
		if (s == 0)
		{
			return false;
		}

		size_t newSize = _cursor + s;
		
		void* b = new char[newSize];
		if (b == NULL)
		{
			return false;
		}
		memset(b, 0, newSize);
		if (_cursor > 0)
		{
			memcpy(b, _buffer, _cursor);
		}

		safeDelete(_buffer);
		_buffer = b;

		_length = newSize;

		return true;
	}

	bool write(void* b, size_t s)
	{
		if (isOverFlow(s))
		{
			if (!reallocate(s * 2))
			{
				return false;
			}
			
		}
		_writeImp(b, s);
		
		return true;
	}

	bool _writeImp(void* b, size_t s)
	{
		if (b == NULL)
		{
			return false;
		}
		memcpy((char*)_buffer + _cursor, (char*)b, s);
		_cursor += s;
		return true;
	}

	size_t getSize()
	{
		if (_cursor == 0)
		{
			return 0;
		}

		return _cursor;
	}

	void* getBuffer()
	{
		return _buffer;
	}
private:
	void*			_buffer;
	unsigned int	_cursor;
	unsigned int	_length;
};

struct sChunk
{
public:
	sChunk()
	{
	}
	~sChunk()
	{
	}
	std::string name;
	MemoryFileWriter buffer;
};

class ChunkSet
{
public:
	std::vector<sChunk*> _chunkSet;
	sChunk*			_currentChunk;

public:
	ChunkSet()
	{
		_currentChunk = NULL;
	}
	~ChunkSet()
	{
		_currentChunk = NULL;
		STLDeleteSequence(_chunkSet);
	}
public:
	bool beginChunk(const std::string& name)
	{
		if (name.size() != 4)
		{
			return false;
		}

		sChunk* c = new sChunk;
		c->name = name;
		_chunkSet.push_back(c);
		_currentChunk = c;

		return true;
	}

	void endChunk()
	{
		_currentChunk = NULL;
	}

	bool write(const char* b, size_t s)
	{
		return write((void*)b, s);
	}

	bool write(void* b, size_t s)
	{
		if (_currentChunk == NULL)
		{
			return false;
		}

		return _currentChunk->buffer.write(b, s);
	}

	void save(std::string& fileName)
	{
		std::ofstream o(fileName.c_str(), std::ios::binary);
		
		for (size_t i = 0; i != _chunkSet.size(); ++i)
		{
			size_t s = _chunkSet[i]->buffer.getSize();

			std::string name = _chunkSet[i]->name;
			char c[4];
			c[0] = name[3];
			c[1] = name[2];
			c[2] = name[1];
			c[3] = name[0];

			o.write(c, 4);
			
			o.write((char*)&s, sizeof(size_t));
			
			o.write((char*)_chunkSet[i]->buffer.getBuffer(), s);
		}
		
		o.close();
	}

	MemoryFileWriter* getCurrentMemoryFileWriter()
	{
		return &_currentChunk->buffer;
	}
};

#endif // __PlatoTreeEnumProc_h__
