/********************************************************************
**	file: 		RainParticle.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __RainParticle_h__
#define __RainParticle_h__

#include "MaxParticleCommon.h"
#include "GenParticle.h"
#include "RainParticleDraw.h"
#include "IMaxParticle.h"
#include "IMaxMaterial.h"
#include "stdmat.h"

class RainParticle : public IMaxParticle 
{
	// from IMaxParticle
public:
	//
	virtual bool isEnable(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Enable, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isVisible(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Visible, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	//
	virtual float getSpeed(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Speed, t, speed, iValid);

		return speed;
	}
	virtual float getVariation(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_SpeedVariation, t, speed, iValid);

		return speed;
	}	
	virtual float getConeAngle(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_ConeAngle, t, speed, iValid);

		return speed;
	}
	virtual float getGravity(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Gravity, t, speed, iValid);

		return speed;
	}
	virtual float getExplosiveForce(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_ExplosiveForce, t, speed, iValid);

		return speed;
	}
	virtual float getLife(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Life, t, speed, iValid);

		return speed;
	}
	virtual float getLifeVariation(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_LifeVariation, t, speed, iValid);

		return speed;
	}
	virtual float getEmissionRate(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_EmissionRate, t, speed, iValid);

		return speed;
	}
	//
	virtual short getInitNum(TimeValue t = 0)
	{
		int st = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_InitialNum, t, st, iValid);

		return st;
	}
	virtual short getLimitNum(TimeValue t = 0)
	{
		int st = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_LimitNum, t, st, iValid);

		return st;
	}
	//
	virtual bool isAttachToEmitter(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_AttachToEmitter, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isMoveWithEmitter(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_MoveWithEmitter, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isForTheSword(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Sword, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	//
	virtual float getInitialAngleForTheSword(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_SwordInitialAngle, t, speed, iValid);

		return speed;
	}
	//
	virtual bool isWander(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Wander, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	//
	virtual float getWanderRadius(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_WanderRadius, t, speed, iValid);

		return speed;
	}
	virtual float getWanderSpeed(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_WanderSpeed, t, speed, iValid);

		return speed;
	}
	virtual float getAspectRatio(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_AspectRatio, t, speed, iValid);

		return speed;
	}
	virtual float getInitialAngleBegin(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_InitialAngleBegin, t, speed, iValid);

		return speed;
	}
	virtual float getInitialAngleEnd(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_InitialAngleEnd, t, speed, iValid);

		return speed;
	}
	virtual float getRotationSpeed(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_RotationSpeed, t, speed, iValid);

		return speed;
	}
	virtual float getRotationSpeedVariation(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_RotationSpeedVariation, t, speed, iValid);

		return speed;
	}
	virtual float getWidth(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_EmitterWidth, t, speed, iValid);

		return speed;
	}
	virtual float getLength(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_EmitterLength, t, speed, iValid);

		return speed;
	}
	virtual float getHeight(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_EmitterHeight, t, speed, iValid);

		return speed;
	}
	//
	virtual const char* getTextureFileName()
	{
		static std::string name;
		name.clear();
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			Texmap* tm = mm->GetSubTexmap(0);
			if (tm && tm->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			{
				// It is -- Access the UV tiling settings at time 0.
				BitmapTex* bm = (BitmapTex*)tm;
				if (bm)
				{
					name = bm->GetMapName();
					MaxSDK::Util::Path filePath(name.c_str());
					name = filePath.StripToLeaf().GetCStr();
				}
			}
		}

		return name.c_str();
	}
	//
	virtual short getBlendMode(TimeValue t = 0)
	{
		short st = 0;
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			st = mm->getBlendMode();
		}

		return st;
	}
	virtual short getTextureRows(TimeValue t = 0)
	{
		short st = 0;
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			st = mm->getRows();
		}

		return st;
	}
	virtual short getTextureCols(TimeValue t = 0)
	{
		short st = 0;
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			st = mm->getCols();
		}

		return st;
	}
	virtual short getChangeStyle(TimeValue t = 0)
	{
		short st = 0;
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			st = mm->getChangeStyle(t);
		}

		return st;
	}
	virtual short getChangeInterval(TimeValue t = 0)
	{
		short st = 0;
		if (_material)
		{
			IMaxMaterial* mm = (IMaxMaterial*)_material;
			st = mm->getChangeInterval();
		}

		return st;
	}
	//
	virtual float getTailLength(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_TailLength, t, speed, iValid);

		return speed;
	}
	virtual float getTailMiddle(TimeValue t = 0)
	{
		float speed = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_TimeMiddle, t, speed, iValid);

		return speed;
	}
	//
	virtual Color getColorStart(TimeValue t = 0)
	{
		Color c;
		if (pblock)
		{
			Interval iValid = FOREVER;
			pblock->GetValue(eParamUID_ColorStart, t, c, iValid);
		}
		
		return c;
	}
	virtual Color getColorMiddle(TimeValue t = 0)
	{
		Color c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_ColorMiddle, t, c, iValid);

		return c;
	}
	virtual Color getColorEnd(TimeValue t = 0)
	{
		Color c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_ColorEnd, t, c, iValid);

		return c;
	}
	//
	virtual Point3 getAlpha(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Alpha, t, c, iValid);

		return c;
	}
	virtual Point3 getScale(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Scale, t, c, iValid);

		return c;
	}
	virtual Point3 getScaleVariation(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_ScaleVariation, t, c, iValid);

		return c;
	}
	//
	virtual bool isFixedSize(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_FixedSize, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	//
	virtual Point3 getHeadLifeSpan(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_HeadLifeSpan, t, c, iValid);

		return c;
	}
	virtual Point3 getHeadDecay(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_HeadDecay, t, c, iValid);

		return c;
	}
	virtual Point3 getTailLifeSpan(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_TailLifeSpan, t, c, iValid);

		return c;
	}
	virtual Point3 getTailDecay(TimeValue t = 0)
	{
		Point3 c;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_TailDecay, t, c, iValid);

		return c;
	}
	//
	virtual bool isHead(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Head, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isTail(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_Tail, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isUnShaded(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_UnShaded, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isUnFogged(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_UnFogged, t, enable, iValid);

		return enable == 0 ? false : true;
	}
	virtual bool isBlockByYO(TimeValue t = 0)
	{
		int enable = 0;
		Interval iValid = FOREVER;
		pblock->GetValue(eParamUID_BlockByY0, t, enable, iValid);

		return enable == 0 ? false : true;
	}

public:
	RainParticleDraw theRainDraw;
	MeshParticleDraw theMeshDraw;
	Mesh*			 pMesh;
	bool			 onlyRenderMarker;

	RainParticle();

	// From BaseObject
	TCHAR *GetObjectName();

	// From GeomObject
	Mesh* GetRenderMesh(TimeValue t, INode *inode, View& view, BOOL& needDelete);
	int IsInstanceDependent();

	// Animatable methods				
	Class_ID ClassID();

	// From ref
	RefTargetHandle Clone(RemapDir& remap);		

	// From SimpleParticle
	MarkerType GetMarkerType();		
	float ParticleSize(TimeValue t,int i);
	int ParticleCenter(TimeValue t,int i);

	int DialogID();
	ParamUIDesc *ParamDesc();
	int DescCount();

	//
	int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
};


#endif // __RainParticle_h__
