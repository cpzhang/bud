/********************************************************************
**	file: 		MaxParticlePBAccessor.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-23
**	purpose:	
*********************************************************************/
#ifndef __MaxParticlePBAccessor_h__
#define __MaxParticlePBAccessor_h__

#include "MaxParticleCommon.h"


class MaxPBAccessor : public PBAccessor
{
public:
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
		MaxParticle* m = (MaxParticle*)owner;
		switch (id)
		{
		default:
			break;
		}
	}

	//

	void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval& valid)    // get into v
	{
		MaxParticle* m = (MaxParticle*)owner;
		switch (id)
		{
		default:
			break;
		}
	}
};

MaxPBAccessor tMaxPBAccessor;

#endif // __MaxParticlePBAccessor_h__
