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
		sKeyFrames<float> explosiveForce;	//17加入
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
		Vec3 particleScalingVar;		//一个随机值，粒子的缩放比例是[particleScaling - particleScalingVar,particleScaling + particleScalingVar]
		bool	fixedSize;
		Vec3 headLifeSpan;
		Vec3 headDecay;
		Vec3 tailLifeSpan;
		Vec3 tailDecay;

		int rows;
		int columns;
		//0 - 顺序，1 - 随机
		short changeStyle;
		//换图的时间间隔
		short changeInterval;
		int textureId;
		int priorityPlane;
		int replaceableId;
		float time;
		float lifeSpan;
		float lifeVar;		//生命值的随机值，比如1.0f，表示生命[life - 1.0f,life + 1.0f]
		float tailLength;
		float aspectRatio;	//粒子图片的宽高比
		float initialAngleBegin;	//粒子初始旋转角度的最小值
		float initialAngleEnd;	//粒子初始旋转角度的最大值，真正的角度是个在最小和最大之间的随机值
		float rotationSpeed;		//粒子旋转速度[-16.0f,16.0f]
		float rotationSpeedVar;		//粒子旋转速度随机值[0,16.0f]

		short initialNum;	//17 - 加入
		short limitNum;		//11 - 加入
		bool attachToEmitter;
		bool moveWithEmitter;	//18 - 加入
		bool forTheSword;		//23 - 加入
		float forTheSwordInitialAngle;	//24 - 加入
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
 
