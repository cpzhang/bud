#pragma once

//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Includes for Plugins
// AUTHOR: 
//***************************************************************************/

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
//SIMPLE TYPE

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define MaxMaterial_CLASS_ID	Class_ID(0x99837aa8, 0x731fb204)


#define NSUBMTL		1 // TODO: number of sub-materials supported by this plugin 

#define PBLOCK_REF	NSUBMTL

enum eBlockID
{
	eBlockID_UnnamedTextureAttibutes,
};


//TODO: Add enums for various parameters
enum eParamID
{ 
	eParamID_TwoSides,
	eParamID_BlendMode,
	eParamID_TcFlowU,
	eParamID_TcFlowV,
	eParamID_TcRotate,
	eParamID_TcAngleReset,
	eParamID_TcPositionReset,
	eParamID_TcFlowSpeedU,
	eParamID_TcFlowSpeedV,
	eParamID_TcRotateSpeed,
	eParamID_Color,
	eParamID_UnnamedTransparency,
	eParamID_UnnamedColor,
	eParamID_TextureRows,
	eParamID_TextureCols,
	eParamID_TextureChangeStyle,
	eParamID_TextureChangeInterval,
	eParamID_DontCastShadow,
	eParamID_Unlit,
	eParamID_NoDepthCheck,
	eParamID_NoDepthWrite,
	eParamID_TexturedChannel,
	eParamID_TextureResolution,
	eParamID_TextureFilter,
	eParamID_Caching,
	eParamID_NodeState,
	eParamID_AdditiveLayer,
	eParamID_TranslucenceCoeff,
	eParamID_DiffuseReflectivity,
	eParamID_Incandescence,
	eParamID_Transparency,
};

enum eBlendMode
{
	eBlendMode_Opaque,
	eBlendMode_Transparent,
	eBlendMode_AlphaBlend,
	eBlendMode_Additive,
	eBlendMode_AdditiveAlpha,
	eBlendMode_Modulate,
	eBlendMode_Size,
};

enum eTexturedChannel
{
	eTexturedChannel_None,
	eTexturedChannel_Color,
	eTexturedChannel_Transparency,
	eTexturedChannel_Incandescence,
	eTexturedChannel_BumpMap,
	eTexturedChannel_CombinedTextures,
	eTexturedChannel_Size
};

enum eTextureChangeStyle
{
	eTextureChangeStyle_Sequence,
	eTextureChangeStyle_Random,
	eTextureChangeStyle_Stop,
	eTextureChangeStyle_Size,
};

enum eTextureResolution
{
	eTextureResolution_Default,
	eTextureResolution_Low,
	eTextureResolution_Medium,
	eTextureResolution_High,
	eTextureResolution_Highest,
	eTextureResolution_Size
};

enum eTextureFilter
{
	eTextureFilter_UseGlobalSetting,
	eTextureFilter_Nearest,
	eTextureFilter_Bilinear,
	eTextureFilter_Mipmap_Nearest,
	eTextureFilter_Mipmap_Linear,
	eTextureFilter_Mipmap_Bilinear,
	eTextureFilter_Mipmap_Trilinear,
	eTextureFilter_Size,
};

enum eNodeState
{
	eNodeState_Normal,
	eNodeState_HasNoEffect,
	eNodeState_Blocking,
	eNodeState_Waiting_Normal,
	eNodeState_Waiting_HasNoEffect,
	eNodeState_Waiting_Blocking,
	eNodeState_Size,
};