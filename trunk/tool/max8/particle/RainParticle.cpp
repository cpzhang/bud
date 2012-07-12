#include "RainParticle.h"
#include "RainParamUIDesc.h"
#include "NullView.h"

ParamUIDesc *RainParticle::ParamDesc()
{
	return tParamUIDesc;
}

int RainParticle::DescCount()
{
	return eParamUID_Size;
}

RefTargetHandle RainParticle::Clone(RemapDir& remap) 
{
	RainParticle* newob = new RainParticle();	
	newob->ReplaceReference(0,remap.CloneRef(pblock));	
	newob->mvalid.SetEmpty();	
	newob->tvalid = FALSE;
	BaseClone(this, newob, remap);
	return newob;
}


MarkerType RainParticle::GetMarkerType() 
{
	parts.SetCustomDraw(&theRainDraw);
	theRainDraw.life = 1;

	return POINT_MRKR;
}


float RainParticle::ParticleSize(TimeValue t,int i)
{
	return 1.0f;
}

int RainParticle::ParticleCenter(TimeValue t,int i)
{
	return PARTCENTER_HEAD;
}

Mesh* RainParticle::GetRenderMesh(TimeValue t, INode *inode, View& view, BOOL& needDelete)
{
	_material = inode->GetMtl();
	Mesh *pm = new Mesh;
	return pm;
}

TCHAR * RainParticle::GetObjectName()
{
	return "Unnamed Particle";
}

int RainParticle::IsInstanceDependent()
{
	 return 1;
}

Class_ID RainParticle::ClassID()
{
	 return tRainParticleCID;
}

int RainParticle::DialogID()
{
	 return IDD_RAINPARAM;
}

RainParticle::RainParticle()
{
	_material = NULL;
	pMesh = NULL;
	onlyRenderMarker = true;
	pblock->SetValue(eParamUID_Enable, 0, 1);
	pblock->SetValue(eParamUID_Visible, 0, 1);
	pblock->SetValue(eParamUID_Speed, 0, 40.0f);
	pblock->SetValue(eParamUID_SpeedVariation, 0, 2.0f);
	pblock->SetValue(eParamUID_Life, 0, 1.0f);
	pblock->SetValue(eParamUID_EmissionRate, 0, 50.0f);
	pblock->SetValue(eParamUID_LimitNum, 0, 32);
	pblock->SetValue(eParamUID_AspectRatio, 0, 1.0f);
	pblock->SetValue(eParamUID_Scale, 0, Point3(10.0f, 10.0f, 10.0f));
	pblock->SetValue(eParamUID_Head, 0, 1);
	pblock->SetValue(eParamUID_UnShaded, 0, 1);
	pblock->SetValue(eParamUID_UnFogged, 0, 1);
	pblock->SetValue(eParamUID_Alpha, 0, Point3(1.0f, 1.0f, 0.0f));
	pblock->SetValue(eParamUID_TimeMiddle, 0, 0.3f);
	pblock->SetValue(eParamUID_ColorStart, 0, Color(1.0f, 1.0f, 1.0f));
	pblock->SetValue(eParamUID_ColorMiddle, 0, Color(1.0f, 1.0f, 1.0f));
	pblock->SetValue(eParamUID_ColorEnd, 0, Color(1.0f, 1.0f, 1.0f));
}

int RainParticle::Display( TimeValue t, INode* inode, ViewExp *vpt, int flags )
{
	if (!OKtoDisplay(t)) 
		return 0;

	GraphicsWindow *gw = vpt->getGW();
	DWORD rlim  = gw->getRndLimits();

	// Draw emitter
	gw->setRndLimits(GW_WIREFRAME | GW_EDGES_ONLY | (rlim & GW_Z_BUFFER) );

	if (inode->Selected())
	{
		gw->setColor( LINE_COLOR, GetSelColor());
	}
	else if(!inode->IsFrozen())
	{
		gw->setColor(LINE_COLOR,GetUIColor(COLOR_PARTICLE_EM));
	}

	Material *mtl = gw->getMaterial();   
	if (mtl && !inode->Selected() && !inode->IsFrozen())
	{
		gw->setColor(LINE_COLOR, mtl->Kd[0], mtl->Kd[1], mtl->Kd[2]);
	}

	gw->setTransform(inode->GetObjTMBeforeWSM(t));  
	mesh.render(gw, mtl, (flags & USE_DAMAGE_RECT) ? &vpt->GetDammageRect() : NULL, COMP_ALL);
	
	return 0;
}
