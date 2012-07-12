#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "simpobj.h"
#include "texutil.h"
#include "particle.h"
//SIMPLE TYPE

//
#include <vector>

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

//--- Parameter map/block descriptors -------------------------------

#define RAINSIZEFACTOR (float(TIME_TICKSPERSEC)/120.0f)
#define PARTICLE_SEED	0x8d6a65bc

#define No_Something
enum eParamUID
{
	eParamUID_Enable,
	eParamUID_Visible,
	eParamUID_Speed,
	eParamUID_SpeedVariation,
	eParamUID_ConeAngle,
	eParamUID_Gravity,
	eParamUID_ExplosiveForce,
	eParamUID_Life,
	eParamUID_LifeVariation,
	eParamUID_EmissionRate,
	eParamUID_InitialNum,
	eParamUID_LimitNum,
	eParamUID_AttachToEmitter,
	eParamUID_MoveWithEmitter,
	eParamUID_Sword,
	eParamUID_SwordInitialAngle,
	eParamUID_Wander,
	eParamUID_WanderRadius,
	eParamUID_WanderSpeed,
	eParamUID_AspectRatio,
	eParamUID_InitialAngleBegin,
	eParamUID_InitialAngleEnd,
	eParamUID_RotationSpeed,
	eParamUID_RotationSpeedVariation,
	eParamUID_EmitterWidth,
	eParamUID_EmitterLength,
	eParamUID_EmitterHeight,
// 	eParamUID_BlendMode,
// 	eParamUID_TextureFile,
// 	eParamUID_TextureRows,
// 	eParamUID_TextureCols,
// 	eParamUID_TextureChangeStyle,
// 	eParamUID_TextureChangeInterval,
	eParamUID_TailLength,
	eParamUID_TimeMiddle,
	eParamUID_ColorStart,
	eParamUID_ColorMiddle,
	eParamUID_ColorEnd,
	eParamUID_Alpha,
	eParamUID_Scale,
	eParamUID_ScaleVariation,
	eParamUID_FixedSize,
	eParamUID_HeadLifeSpan,
	eParamUID_HeadDecay,
	eParamUID_TailLifeSpan,
	eParamUID_TailDecay,
	eParamUID_Head,
	eParamUID_Tail,
	eParamUID_UnShaded,
	eParamUID_UnFogged,
	eParamUID_BlockByY0,

	eParamUID_Size,
};

// render types
enum eRenderType
{
	eRenderType_Face,
	eRenderType_Size,
};

#define A_RENDER			A_PLUGIN1

static const unsigned int tMaxParticlesCount = 500000;

// Parameters
#define PBLOCK_LENGTH_RAIN 14

#define NUM_OLDVERSIONS	3

// Current version
enum eVersion
{
	eVersion_Current,
	eVersion_Size,
};

#define MAX_EVAL_TIME_DELTA		TIME_TICKSPERSEC*1000


#define VEL_SCALE	(-0.01f*1200.0f/float(TIME_TICKSPERSEC))
#define VAR_SCALE	(0.01f*1200.0f/float(TIME_TICKSPERSEC))

// The ratio of the base of the tetrahedron to its height
#define BASEFACTOR	5.0f
#define THIRD 		(1.0f/3.0f)
#define SIXTH 		(1.0f/6.0f)
#define TWOTHIRD 	(2.0f/3.0f)
#define FIVESIXTHS 	(5.0f/6.0f)