/********************************************************************
**	file: 		BirthRateDimension.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __BirthRateDimension_h__
#define __BirthRateDimension_h__

#include "MaxParticleCommon.h"

class BirthRateDimension : public ParamDimension 
{
public:
	DimType DimensionType()
	{
		return DIM_CUSTOM;
	}
	
	float Convert(float value)
	{
		return value*(float)GetTicksPerFrame();
	}

	float UnConvert(float value) 
	{
		return value/(float)GetTicksPerFrame();
	}
};

static BirthRateDimension theBirthRateDim;


#endif // __BirthRateDimension_h__
