//==========================================================================
/**
* @file	  : ParticleSystem2.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-7-2   9:56
* purpose : 
*/
//==========================================================================

#ifndef __ParticleSystem2_h__
#define __ParticleSystem2_h__

#include "Appearance.h"
#include "KeyFrames.h"
#include "RenderQueue.h"

namespace Euclid
{
	struct sParticleEmitter
	{
		sParticleEmitter()
		{
			filterMode = BM_OPAQUE;

			speed.setStaticData(0.0f);
			variation.setStaticData(0.0f);
			latitude.setStaticData(0.0f);
			gravity.setStaticData(0.0f);
			explosiveForce.setStaticData(0.0f);
			visibility.setStaticData(1.0f);
			emissionRate.setStaticData(0.0f);
			width.setStaticData(0.0f);
			length.setStaticData(0.0f);
			height.setStaticData(0.0f);

			segmentColor1 = Color3(1.0f, 1.0f, 1.0f);
			segmentColor2 = Color3(1.0f, 1.0f, 1.0f);
			segmentColor3 = Color3(1.0f, 1.0f, 1.0f);
			alpha = Vec3(255.0f, 255.0f, 255.0f);
			particleScaling = Vec3(1.0f, 1.0f, 1.0f);
			particleScalingVar = Vec3(0.0f,0.0f,0.0f);
			fixedSize = false;
			headLifeSpan = Vec3(0.0f, 0.0f, 1.0f);
			headDecay = Vec3(0.0f, 0.0f, 1.0f);
			tailLifeSpan = Vec3(0.0f, 0.0f, 1.0f);
			tailDecay = Vec3(0.0f, 0.0f, 1.0f);

			rows = 1;
			columns = 1;
			changeStyle = 0;
			changeInterval = 1000;
			textureId = -1;
			priorityPlane = 0;
			replaceableId = 0;
			time = 0.0f;
			lifeSpan = 0.0f;
			lifeVar = 0.0f;
			tailLength = 0.0f;

			aspectRatio = 1.0f;
			initialAngleBegin = 0.0f;
			initialAngleEnd = 0.0f;
			rotationSpeed = 0.0f;
			rotationSpeedVar = 0.0f;

			initialNum = 0;
			limitNum = 32;
			attachToEmitter = false;
			moveWithEmitter = false;
			forTheSword = false;
			forTheSwordInitialAngle = 0.0f;
			wander = false;
			wanderRadius = 0.0f;
			wanderSpeed = 0.0f;

			sortPrimitivesFarZ = false;
			lineEmitter = false;
			modelSpace = false;
			alphaKey = false;
			unshaded = false;
			unfogged = false;
			blockByY0 = false;
			xyQuad = false;
			squirt = false;
			head = false;
			tail = false;

			boneObjectId = -1;
		}

		eBlendMode filterMode;

		sKeyFrames<float> speed;
		sKeyFrames<float> variation;
		sKeyFrames<float> latitude;
		sKeyFrames<float> gravity;
		sKeyFrames<float> explosiveForce;	//17����
		sKeyFrames<float> visibility;
		sKeyFrames<float> emissionRate;
		sKeyFrames<float> width;
		sKeyFrames<float> length;
		sKeyFrames<float> height;

		Color3 segmentColor1;
		Color3 segmentColor2;
		Color3 segmentColor3;
		Vec3 alpha;
		Vec3 particleScaling;
		Vec3 particleScalingVar;		//һ�����ֵ�����ӵ����ű�����[particleScaling - particleScalingVar,particleScaling + particleScalingVar]
		bool	fixedSize;
		Vec3 headLifeSpan;
		Vec3 headDecay;
		Vec3 tailLifeSpan;
		Vec3 tailDecay;

		int rows;
		int columns;
		//0 - ˳��1 - ���
		short changeStyle;
		//��ͼ��ʱ����
		short changeInterval;
		int textureId;
		int priorityPlane;
		int replaceableId;
		float time;
		float lifeSpan;
		float lifeVar;		//����ֵ�����ֵ������1.0f����ʾ����[life - 1.0f,life + 1.0f]
		float tailLength;
		float aspectRatio;	//����ͼƬ�Ŀ�߱�
		float initialAngleBegin;	//���ӳ�ʼ��ת�Ƕȵ���Сֵ
		float initialAngleEnd;	//���ӳ�ʼ��ת�Ƕȵ����ֵ�������ĽǶ��Ǹ�����С�����֮������ֵ
		float rotationSpeed;		//������ת�ٶ�[-16.0f,16.0f]
		float rotationSpeedVar;		//������ת�ٶ����ֵ[0,16.0f]

		short initialNum;	//17 - ����
		short limitNum;		//11 - ����
		bool attachToEmitter;
		bool moveWithEmitter;	//18 - ����
		bool forTheSword;		//23 - ����
		float forTheSwordInitialAngle;	//24 - ����
		bool wander;		//25
		float wanderRadius;	//25
		float wanderSpeed;	//25

		bool sortPrimitivesFarZ;
		bool lineEmitter;
		bool modelSpace;
		bool alphaKey;
		bool unshaded;
		bool unfogged;
		bool blockByY0;
		bool xyQuad;
		bool squirt;
		bool head;
		bool tail;

		std::string textureFilename;
		int boneObjectId;
	};

	class ParticleSystem2 : public IRenderable
	{
	public:
		virtual void  renderGeometry();

		virtual const Appearance& getAppearance() const;

		virtual const Mat4& getWorldTransform() const;

	public:
		ParticleSystem2();
		~ParticleSystem2();

	public:
		void update(const AnimationTime& at, float delta, bool );
	};
}

#endif // __ParticleSystem2_h__
 
