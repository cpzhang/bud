/********************************************************************
**	file: 		MaxMaterial.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __IMaxMaterial_h__
#define __IMaxMaterial_h__
#include "maxtypes.h"

#define MaxMaterial_CLASS_ID	Class_ID(0x99837aa8, 0x731fb204)

class IMaxMaterial : public Mtl, public IReshading
{
public:
	//
	virtual bool isTwoSided() = NULL;
	//
	virtual bool isTcFlowU() = NULL;
	//
	virtual bool isTcFlowV() = NULL;
	//
	virtual bool isTcRotate() = NULL;
	//
	virtual bool isTcAngleReset() = NULL;
	//
	virtual bool isTcPositionReset() = NULL;
	//
	virtual float getTcFlowSpeedU(TimeValue time) = NULL;
	//
	virtual float getTcFlowSpeedV(TimeValue time) = NULL;
	//
	virtual float getTcRotateSpeed(TimeValue time) = NULL;
	//
	virtual float getTransparency(TimeValue time) = NULL;
	//
	virtual Point3 getColor(TimeValue time) = NULL;
	//
	virtual int getRows() = NULL;
	//
	virtual int getCols() = NULL;
	//
	virtual int getChangeStyle(TimeValue time) = NULL;
	//
	virtual int getChangeInterval() = NULL;
	//
	virtual bool dontCastShadow() = NULL;
	//
	virtual bool isUnlit() = NULL;
	//
	virtual bool isNoDepthWrite() = NULL;
	//
	virtual bool isNoDepthCheck() = NULL;
	//
	virtual int isAdditivelayer() = NULL;
	//
	virtual unsigned char getBlendMode() = NULL;
};


#endif // __IMaxMaterial_h__
