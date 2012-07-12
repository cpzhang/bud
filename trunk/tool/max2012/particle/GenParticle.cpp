#include "GenParticle.h"
#include "RainParamUIDesc.h"
#include "ParticlePostLoadCallback.h"
#include "MaxParticleDlgProc.h"
#include "CollisionCollection.h"
#include "EmitterCreateCallback.h"
#include "ParamBlockDescID.h"
#include "IMaxParticle.h"

GenParticle::GenParticle()
{
	ReplaceReference(0, CreateParameterBlock(tParamBlockDescID, eParamUID_Size, eVersion_Current));
	assert(pblock);
}

void GenParticle::RescaleWorldUnits(float f)
{
	if (TestAFlag(A_WORK1))
		return;

	// Call the base class's rescale (this sets the A_WORK1 flag)
	SimpleParticle::RescaleWorldUnits(f);
}


IOResult GenParticle::Load(ILoad *iload)
{
	if (ClassID() == tRainParticleCID) 
	{
		iload->RegisterPostLoadCallback(
			new ParticlePostLoadCallback(
			new ParamBlockPLCB(tOldParamVersions, eVersion_Current, &curVersionRain, this,0)));
	} 
	
	return IO_OK;
}

TimeValue GenParticle::ParticleLife(TimeValue t, int i)
{
	TimeValue life;	
	return life;
}

Point3 GenParticle::ParticlePosition(TimeValue t,int i)
{
	return parts.points[i];
}

Point3 GenParticle::ParticleVelocity(TimeValue t,int i)
{
	return parts.vels[i];
}


int GenParticle::RenderBegin(TimeValue t, ULONG flags)
{
	SetAFlag(A_RENDER);
	ParticleInvalid();		
	NotifyDependents(FOREVER,PART_ALL,REFMSG_CHANGE);
	return 0;
}

int GenParticle::RenderEnd(TimeValue t)
{
	ClearAFlag(A_RENDER);
	ParticleInvalid();		
	NotifyDependents(FOREVER,PART_ALL,REFMSG_CHANGE);
	return 0;
}

int	GenParticle::GetParticleCount()
{
	int c;

	if (c > tMaxParticlesCount)
	{
		c = tMaxParticlesCount;
	}
	
	return c;
}

void GenParticle::BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev)
{
	SimpleParticle::BeginEditParams(ip,flags,prev);
	if (pmapParam) 
	{
		pmapParam->SetParamBlock(pblock);
	}
	else
	{		
		pmapParam = CreateCPParamMap(
			ParamDesc(),DescCount(),
			pblock,
			ip,
			hInstance,
			MAKEINTRESOURCE(DialogID()),
			"params",
			0);		
	}

	if (pmapParam1)
	{
		pmapParam1->SetParamBlock(pblock);
	}
	else
	{
		pmapParam1 = CreateCPParamMap(
			ParamDesc(),DescCount(),
			pblock,
			ip,
			hInstance,
			MAKEINTRESOURCE(IDD_RAINPARAM1),
			"params1",
			0);		
	}
//	pmapParam->SetUserDlgProc(new ParticleDlgProc(this));
}

void GenParticle::EndEditParams(IObjParam *ip, ULONG flags,Animatable *next)
{
	SimpleParticle::EndEditParams(ip,flags,next);

	if (flags&END_EDIT_REMOVEUI) {
		DestroyCPParamMap(pmapParam);
		pmapParam  = NULL;
	}
}

void GenParticle::MapKeys(TimeMap *map,DWORD flags)
{
	Animatable::MapKeys(map,flags);
}

Interval GenParticle::GetValidity(TimeValue t)
{
	// For now...
	return Interval(t,t);
}


void GenParticle::BuildEmitter(TimeValue t, Mesh& amesh)
{
	float width, height;
	mvalid = FOREVER;
	pblock->GetValue(eParamUID_EmitterWidth, t, width, mvalid);
	pblock->GetValue(eParamUID_EmitterHeight, t, height, mvalid);
	width  *= 0.5f;
	height *= 0.5f;

	mesh.setNumVerts(7);
	mesh.setNumFaces(6);
	mesh.setVert(0, Point3(-width,-height, 0.0f));
	mesh.setVert(1, Point3( width,-height, 0.0f));
	mesh.setVert(2, Point3( width, height, 0.0f));
	mesh.setVert(3, Point3(-width, height, 0.0f));
	mesh.setVert(4, Point3(  0.0f,   0.0f, 0.0f));
	mesh.setVert(5, Point3(  0.0f,   0.0f, -(width + height)/2.0f));
	mesh.setVert(6, Point3(  0.0f,   0.0f, 0.0f));

	mesh.faces[0].setEdgeVisFlags(1,0,1);
	mesh.faces[0].setSmGroup(1);
	mesh.faces[0].setVerts(0,1,3);

	mesh.faces[1].setEdgeVisFlags(1,1,0);
	mesh.faces[1].setSmGroup(1);
	mesh.faces[1].setVerts(1,2,3);

	mesh.faces[2].setEdgeVisFlags(1,1,0);
	mesh.faces[2].setSmGroup(1);
	mesh.faces[2].setVerts(4,5,6);

	mesh.faces[3].setEdgeVisFlags(1,0,1);
	mesh.faces[3].setSmGroup(1);
	mesh.faces[3].setVerts(0,3,1);

	mesh.faces[4].setEdgeVisFlags(0,1,1);
	mesh.faces[4].setSmGroup(1);
	mesh.faces[4].setVerts(1,3,2);

	mesh.faces[5].setEdgeVisFlags(1,0,0);
	mesh.faces[5].setSmGroup(1);
	mesh.faces[5].setVerts(5,4,6);

	mesh.InvalidateGeomCache();
}


int GenParticle::CountLive()
{
	int c=0;
	for (int i=0; i<parts.Count(); i++) {
		if (parts.Alive(i)) c++;
	}
	return c;
}


void GenParticle::ComputeParticleStart(TimeValue t0,INode *node)
{
	_material = node->GetMtl();
	int c = GetParticleCount();	
	parts.SetCount(c,PARTICLE_VELS|PARTICLE_AGES);
	for (int i=0; i<parts.Count(); i++) {
		parts.ages[i] = -1;
	}
	tvalid = t0;
	valid  = TRUE;
}


void GenParticle::BirthParticle(INode *node,TimeValue bt,int index)
{
	_material = node->GetMtl();
}


void GenParticle::UpdateParticles(TimeValue t,INode *node)
{
	_material = node->GetMtl();	
}

CreateMouseCallBack* GenParticle::GetCreateMouseCallBack() 
{
	tEmitterCreateCallback.rain = this;
	return &tEmitterCreateCallback;
}

void GenParticle::InvalidateUI()
{
	if (pmapParam) pmapParam->Invalidate();
}

BOOL GenParticle::EmitterVisible()
{
	return TRUE;
}

ParamDimension *GenParticle::GetParameterDim(int pbIndex) 
{
	return defaultDim;
}

TSTR GenParticle::GetParameterName(int pbIndex)
{
	switch (pbIndex) 
	{
	case eParamUID_Visible:
		return "Visible";

	case eParamUID_Speed:
		return "Speed";

	case eParamUID_SpeedVariation:
		return "Speed Variation";

	case eParamUID_ConeAngle:
		return "Cone Angle";

	case eParamUID_Gravity:
		return "Gravity";

	case eParamUID_ExplosiveForce:
		return "Explosive Force";

	case eParamUID_EmissionRate:
		return "Emission Rate";

	case eParamUID_EmitterWidth:
		return "Emitter Width";

	case eParamUID_EmitterLength:
		return "Emitter Length";

	case eParamUID_EmitterHeight:
		return "Emitter Height";

	default: 	
		return TSTR(_T(""));
	}
}


Mesh *GenParticle::GetFacingRenderMesh(TimeValue t, INode *inode, View& view, BOOL& needDelete)
{
	_material = inode->GetMtl();
	Mesh *pm = new Mesh;	
	return pm;
}


void GenParticle::GetWorldBoundBox(TimeValue t, INode *inode, ViewExp* vpt, Box3& box)
{
	_material = inode->GetMtl();
}

int GenParticle::HitTest( TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt )
{
	float width, height;
	mvalid = FOREVER;
	pblock->GetValue(eParamUID_EmitterWidth, t, width, mvalid);
	pblock->GetValue(eParamUID_EmitterHeight, t, height, mvalid);
	switch(type)
	{
	case HITTYPE_POINT:
		{

		}break;
	case HITTYPE_BOX:
		{

		}break;
	}
	switch(flags)
	{
	case HIT_SELONLY:
		{

		}break;
	}
	//
	BOOL doupdate=((t!=tvalid)||!valid);
	if (doupdate)
		Update(t,inode);

	Point2 pt( (float)p[0].x, (float)p[0].y );
	HitRegion hitRegion;
	GraphicsWindow *gw = vpt->getGW();  
	MakeHitRegion(hitRegion, type, crossing, 4, p);
	DWORD rlim  = gw->getRndLimits();
	int res;
	if (EmitterVisible()) {
		gw->setRndLimits((rlim|GW_PICK|GW_WIREFRAME) 
			& ~(GW_ILLUM|GW_BACKCULL|GW_FLAT|GW_SPECULAR));
		gw->setTransform(inode->GetObjTMBeforeWSM(t));
		res = mesh.select(gw, NULL, &hitRegion, flags & HIT_ABORTONHIT);

		gw->setRndLimits(rlim);
	} 
// 	else 
// 	{
// 		res = 0;
// 	}
	return res;


	return 0;
}

IParamMap * GenParticle::pmapParam;

//--- GenParticle Methods--------------------------------------------

void DL_CacheData( ParticleSys *p0,ParticleSys *p1 )
{
	p1->points.SetCount(p0->points.Count());

	if (p0->points.Count()>0)
	{
		for (int pc=0;pc<p0->points.Count();pc++)
		{
			p1->points[pc]=p0->points[pc];
		}

		p1->ages.SetCount(p0->ages.Count());
		
		for (int pc=0;pc<p0->ages.Count();pc++)
		{
			p1->ages[pc]=p0->ages[pc];
		}
		
		p1->radius.SetCount(p0->radius.Count());
		
		for (int pc=0;pc<p0->radius.Count();pc++)
		{
			p1->radius[pc]=p0->radius[pc];
		}
		
		p1->tension.SetCount(p0->tension.Count());
		
		for (int pc=0;pc<p0->tension.Count();pc++)
		{
			p1->tension[pc]=p0->tension[pc];
		}
	}
}

