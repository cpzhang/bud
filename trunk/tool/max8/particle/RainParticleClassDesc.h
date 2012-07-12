/********************************************************************
**	file: 		RainParticleClassDesc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __RainParticleClassDesc_h__
#define __RainParticleClassDesc_h__

#include "MaxParticleCommon.h"
#include "RainParticle.h"

class RainClassDesc:public ClassDesc {
public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) {return new RainParticle;}
	const TCHAR *	ClassName() {return "Unnamed";}
	SClass_ID		SuperClassID() {return GEOMOBJECT_CLASS_ID;}
	Class_ID		ClassID() {return tRainParticleCID;}
	const TCHAR* 	Category() {return "Particle Systems";}
};


static RainClassDesc rainDesc;

ClassDesc* GetRainDesc()
{
	return &rainDesc;
}

#endif // __RainParticleClassDesc_h__