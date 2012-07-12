/********************************************************************
**	file: 		ParamBlockDescID.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-28
**	purpose:	
*********************************************************************/
#ifndef __ParamBlockDescID_h__
#define __ParamBlockDescID_h__

#include "MaxParticleCommon.h"

static ParamBlockDescID tParamBlockDescID[] = 
{
	{ TYPE_INT,		NULL, FALSE,	eParamUID_Enable},
	{ TYPE_INT,		NULL, TRUE,		eParamUID_Visible},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_Speed},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_SpeedVariation},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_ConeAngle},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_Gravity},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_ExplosiveForce},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_Life},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_LifeVariation},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_EmissionRate},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_InitialNum},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_LimitNum},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_AttachToEmitter},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_MoveWithEmitter},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_Sword},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_SwordInitialAngle},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_Wander},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_WanderRadius},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_WanderSpeed},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_AspectRatio},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_InitialAngleBegin},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_InitialAngleEnd},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_RotationSpeed},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_RotationSpeedVariation},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_EmitterWidth},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_EmitterLength},
	{ TYPE_FLOAT,	NULL, TRUE,		eParamUID_EmitterHeight},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_TailLength},
	{ TYPE_FLOAT,	NULL, FALSE,	eParamUID_TimeMiddle},
	{ TYPE_RGBA,	NULL, FALSE,	eParamUID_ColorStart},
	{ TYPE_RGBA,	NULL, FALSE,	eParamUID_ColorMiddle},
	{ TYPE_RGBA,	NULL, FALSE,	eParamUID_ColorEnd},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_Alpha},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_Scale},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_ScaleVariation},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_FixedSize},
#ifdef No_Something
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_HeadLifeSpan},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_HeadDecay},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_TailLifeSpan},
	{ TYPE_POINT3,	NULL, FALSE,	eParamUID_TailDecay},
#endif
	{ TYPE_INT,		NULL, FALSE,	eParamUID_Head},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_Tail},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_UnShaded},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_UnFogged},
	{ TYPE_INT,		NULL, FALSE,	eParamUID_BlockByY0},
};

// Array of old versions
static ParamVersionDesc* tOldParamVersions = NULL;

static ParamVersionDesc curVersionRain(tParamBlockDescID, eParamUID_Size, eVersion_Current);


#endif // __ParamBlockDescID_h__
