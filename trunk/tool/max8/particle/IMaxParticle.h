/********************************************************************
**	file: 		IMaxParticle.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-28
**	purpose:	
*********************************************************************/
#ifndef __IMaxParticle_h__
#define __IMaxParticle_h__

#include "GenParticle.h"

static const Class_ID tRainParticleCID(0x516b1695, 0x344a17c2);

class IMaxParticle : public GenParticle
{
public:
	//
	virtual bool isEnable(TimeValue t = 0) = NULL;
	virtual bool isVisible(TimeValue t = 0) = NULL;
	//
	virtual float getSpeed(TimeValue t = 0) = NULL;
	virtual float getVariation(TimeValue t = 0) = NULL;	
	virtual float getConeAngle(TimeValue t = 0) = NULL;
	virtual float getGravity(TimeValue t = 0) = NULL;
	virtual float getExplosiveForce(TimeValue t = 0) = NULL;
	virtual float getLife(TimeValue t = 0) = NULL;
	virtual float getLifeVariation(TimeValue t = 0) = NULL;
	virtual float getEmissionRate(TimeValue t = 0) = NULL;
	//
	virtual short getInitNum(TimeValue t = 0) = NULL;
	virtual short getLimitNum(TimeValue t = 0) = NULL;
	//
	virtual bool isAttachToEmitter(TimeValue t = 0) = NULL;
	virtual bool isMoveWithEmitter(TimeValue t = 0) = NULL;
	virtual bool isForTheSword(TimeValue t = 0) = NULL;
	//
	virtual float getInitialAngleForTheSword(TimeValue t = 0) = NULL;
	//
	virtual bool isWander(TimeValue t = 0) = NULL;
	//
	virtual float getWanderRadius(TimeValue t = 0) = NULL;
	virtual float getWanderSpeed(TimeValue t = 0) = NULL;
	virtual float getAspectRatio(TimeValue t = 0) = NULL;
	virtual float getInitialAngleBegin(TimeValue t = 0) = NULL;
	virtual float getInitialAngleEnd(TimeValue t = 0) = NULL;
	virtual float getRotationSpeed(TimeValue t = 0) = NULL;
	virtual float getRotationSpeedVariation(TimeValue t = 0) = NULL;
	virtual float getWidth(TimeValue t = 0) = NULL;
	virtual float getLength(TimeValue t = 0) = NULL;
	virtual float getHeight(TimeValue t = 0) = NULL;
	//
	virtual short getBlendMode(TimeValue t = 0) = NULL;
	virtual short getTextureRows(TimeValue t = 0) = NULL;
	virtual short getTextureCols(TimeValue t = 0) = NULL;
	virtual short getChangeStyle(TimeValue t = 0) = NULL;
	virtual short getChangeInterval(TimeValue t = 0) = NULL;
	//
	virtual float getTailLength(TimeValue t = 0) = NULL;
	virtual float getTailMiddle(TimeValue t = 0) = NULL;
	//
	virtual Color getColorStart(TimeValue t = 0) = NULL;
	virtual Color getColorMiddle(TimeValue t = 0) = NULL;
	virtual Color getColorEnd(TimeValue t = 0) = NULL;
	//
	virtual Point3 getAlpha(TimeValue t = 0) = NULL;
	virtual Point3 getScale(TimeValue t = 0) = NULL;
	virtual Point3 getScaleVariation(TimeValue t = 0) = NULL;
	//
	virtual bool isFixedSize(TimeValue t = 0) = NULL;
	//
	virtual Point3 getHeadLifeSpan(TimeValue t = 0) = NULL;
	virtual Point3 getHeadDecay(TimeValue t = 0) = NULL;
	virtual Point3 getTailLifeSpan(TimeValue t = 0) = NULL;
	virtual Point3 getTailDecay(TimeValue t = 0) = NULL;
	//
	virtual bool isHead(TimeValue t = 0) = NULL;
	virtual bool isTail(TimeValue t = 0) = NULL;
	virtual bool isUnShaded(TimeValue t = 0) = NULL;
	virtual bool isUnFogged(TimeValue t = 0) = NULL;
	virtual bool isBlockByYO(TimeValue t = 0) = NULL;
	//
	virtual const char* getTextureFileName() = NULL;
};

#endif // __IMaxParticle_h__
