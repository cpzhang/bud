/********************************************************************
**	file: 		RainParticleDraw.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __RainParticleDraw_h__
#define __RainParticleDraw_h__

#include "MaxParticleCommon.h"

class RainParticleDraw : public CustomParticleDisplay 
{
public:
	TimeValue life;
	BOOL DrawParticle(GraphicsWindow *gw,ParticleSys &parts,int i);
};

class RainParticle;
class MeshParticleDraw : public CustomParticleDisplay 
{
public:
	TimeValue life;
	RainParticle* sp;
	BOOL DrawParticle(GraphicsWindow *gw,ParticleSys &parts,int i);
};

#endif // __RainParticleDraw_h__
