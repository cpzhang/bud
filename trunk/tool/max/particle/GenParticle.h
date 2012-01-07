/********************************************************************
**	file: 		GenParticle.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __GenParticle_h__
#define __GenParticle_h__

#include "simpobj.h"
#include "iparamb2.h"
#include "iparamm2.h"

class GenParticle : public SimpleParticle
{
public:
	static IParamMap *pmapParam;		
	IParamMap *pmapParam1;		
	int stepSize;
	Mtl*			 _material;

	GenParticle();
	void BirthParticle(INode *node,TimeValue bt,int index);
	void ComputeParticleStart(TimeValue t0,INode *node);
	int CountLive();
	Mesh *GetFacingRenderMesh(TimeValue t, INode *inode, View& view, BOOL& needDelete);
	int	GetParticleCount();				
	TimeValue ParticleLife(TimeValue t, int i);
	void RescaleWorldUnits(float f);

	// From BaseObject
	CreateMouseCallBack* GetCreateMouseCallBack();
	void BeginEditParams( IObjParam *ip, ULONG flags,Animatable *prev);		
	void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);		
	void GetWorldBoundBox(TimeValue t, INode *inode, ViewExp* vpt, Box3& box);

	// Animatable methods		
	void DeleteThis() {delete this;}
	IOResult Load(ILoad *iload);
	void MapKeys(TimeMap *map,DWORD flags);
	int RenderBegin(TimeValue t, ULONG flags);		
	int RenderEnd(TimeValue t);

	// From SimpleParticle
	void UpdateParticles(TimeValue t,INode *node);
	void BuildEmitter(TimeValue t, Mesh& amesh);
	Interval GetValidity(TimeValue t);		
	void InvalidateUI();
	ParamDimension *GetParameterDim(int pbIndex);
	TSTR GetParameterName(int pbIndex);
	BOOL EmitterVisible();		
	Point3 ParticlePosition(TimeValue t,int i);
	Point3 ParticleVelocity(TimeValue t,int i);		
	virtual int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2* p, ViewExp* vpt); 

	virtual int DialogID()=0;
	virtual ParamUIDesc *ParamDesc()=0;
	virtual int DescCount()=0;
};


void DL_CacheData(ParticleSys *p0,ParticleSys *p1);

// 
// static float CompParticleSize( TimeValue age, TimeValue life, float size )
// {
// 	float u = float(age)/float(life);
// 	if (u > 0.75f) 
// 	{
// 		return (1.0f-smoothstep(0.75f,1.0f,u)) * size;
// 	}
// 	
// 	return size;
// }
// 
// static int Parity(Matrix3 tm)
// {
// 	Point3 v = tm.GetRow(0) ^ tm.GetRow(1);
// 	if (DotProd(v,tm.GetRow(2)) < 0.0f) 
// 	{
// 		return 1;
// 	}
// 
// 	return 0;
// }
// 
// 
// static void FlipAllMeshFaces(Mesh *mesh)
// {
// 	for (int i=0; i<mesh->getNumFaces(); i++) 
// 	{
// 		mesh->FlipNormal(i);
// 	}
// }

#endif // __GenParticle_h__
