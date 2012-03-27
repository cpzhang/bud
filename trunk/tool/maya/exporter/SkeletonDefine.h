#pragma once

typedef struct tranTag
{
	float m_mat[4][3];
} tranMatrix;

struct keyframeTranslation
{
	int time;
	float v[3]; 
};

struct keyframeRotation
{
	int time;
	float q[4];
};

struct keyframeScale
{
	int time;
	float v[3];
};

struct RibbonSystemData
{
	bool visible;
	float above;
	float below;
	short edgePerSecond;
	float edgeLife;
	float gravity;
	short rows;
	short cols;
	short slot;
	float3 color;
	float alpha;
	short blendMode;
	std::string textureFilename;

	std::vector<keyframeT<bool> > keyframesVisible;
	std::vector<keyframeT<float> > keyframeAbove;
	std::vector<keyframeT<float> > keyframeBelow;
	std::vector<keyframeT<short> > keyframeSlot;
	std::vector<keyframeT<float3> > keyframeColor;
	std::vector<keyframeT<float> > keyframeAlpha;
};

struct ParticleSystemData
{
	bool visible;
	float speed;
	float variation;
	float coneAngle;
	float gravity;
	//17 - ����
	float explosiveForce;
	float life;
	//5 - ����
	float lifeVar;		//����ֵ�����ֵ������1.0f����ʾ����[life - 1.0f,life + 1.0f]
	float emissionRate;
	//17 - ����
	short initialNum;
	//11 - ����
	short limitNum;
	//13 - ����
	bool attachToEmitter;
	//18 - ����
	bool moveWithEmitter;
	//23 - ����
	bool forTheSword;
	//24 - ����
	float forTheSwordInitialAngle;
	//25 - ����
	bool wander;
	//25
	float wanderRadius;
	//25
	float wanderSpeed;
	//5 - ����
	float aspectRatio;
	//6 - ����
	float initialAngleBegin;
	//6 - ����
	float initialAngleEnd;
	//6 - ����
	float rotationSpeed;
	//15 - ����
	float rotationSpeedVar;
	float width;
	float length;
	//16 - ����
	float height;
	short blendMode;
	std::string textureFilename;
	short textureRows;
	short textureCols;
	//6 - ����
	short changeStyle;
	//6 - ����
	short changeInterval;
	float tailLength;
	float timeMiddle;
	float3 colorStart;
	float3 colorMiddle;
	float3 colorEnd;
	float3 alpha;
	float3 scale;
	//5 - ����
	float3 scaleVar;
	//21 - ����
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
	std::vector<keyframeT<float> > keyframesHeight;	//�汾��16����
};

struct joint
{
	joint()
	{
		hasParticleSystem = false;
		hasRibbonSystem = false;
	}

	MString name;
	int id;
	MMatrix localMatrix;
	MMatrix worldMatrix;
	int parentIndex;
	tranMatrix tran;
	MDagPath jointDag;

	std::vector<keyframeTranslation> keyframesTranslation;
	std::vector<keyframeRotation> keyframesRotation;
	std::vector<keyframeScale> keyframesScale;

	//ParticleSystem,RibbonSystem
	bool	hasParticleSystem;
	bool	hasRibbonSystem;
	RibbonSystemData	ribbon;
	ParticleSystemData	particle;
};

typedef struct animationTag
{
	MString name;
	int startTime;
	int endTime;
} animation;