/********************************************************************
**	file: 		MaxParamBlockDesc2s.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __MaxParamBlockDesc2s_h__
#define __MaxParamBlockDesc2s_h__

#include "MaxMaterialClassDesc.h"
#include "MaxDlgProc.h"
#include "MaxPBAccessor.h"

static ParamBlockDesc2 tParamBlockDesc2( 
	//
	eBlockID_UnnamedTextureAttibutes, _T("params"),  0, GetMaxMaterialDesc(), P_AUTO_CONSTRUCT + P_AUTO_UI, eBlockID_UnnamedTextureAttibutes, 
  
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, &tMaxMaterialDlgProc,
	
	// params

	// tow sides
	eParamID_TwoSides, _T("Two Sides"), TYPE_BOOL, 0, IDS_TWOSIDES,
	p_default,	true,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK1,
	end,

	// blend mode
	eParamID_BlendMode, _T("Blend Mode"), TYPE_INT, 0, IDS_BLENDMODE,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO1, eBlendMode_Size, IDS_BLENDMODE_OPAQUE, IDS_BLENDMODE_TRANSPARENT, IDS_BLENDMODE_ALPHABLEND,
													  IDS_BLENDMODE_ADDITIVE, IDS_BLENDMODE_ADDITIVEALPHA, IDS_BLENDMODE_MODULATE,	
	end,

	// tc flow u
	eParamID_TcFlowU, _T("Tc Flow U"), TYPE_BOOL, 0, IDS_TCFLOWU,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK2,
	end,

	// tc flow v
	eParamID_TcFlowV, _T("Tc Flow V"), TYPE_BOOL, 0, IDS_TCFLOWV,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK3,
	end,

	// tc rotate
	eParamID_TcRotate, _T("Tc Rotate"), TYPE_BOOL, 0, IDS_TCROTATE,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK4,
	end,

	// tc angle reset
	eParamID_TcAngleReset, _T("Tc Angle Reset"), TYPE_BOOL, 0, IDS_TCANGLERESET,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK5,
	end,

	// tc angle reset
	eParamID_TcPositionReset, _T("Tc Position Reset"), TYPE_BOOL, 0, IDS_TCPOSITIONRESET,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK6,
	end,

	// tc flow speed u
	eParamID_TcFlowSpeedU, _T("Tc Flow Speed U"), TYPE_FLOAT, P_ANIMATABLE, IDS_TCFLOWSPEEDU,
	p_default,	0.0f,
	p_range,	-32.0f, 32.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT4,	IDC_SPIN1, 0.01f,
	end,

	// tc flow speed v
	eParamID_TcFlowSpeedV, _T("Tc Flow Speed V"), TYPE_FLOAT, P_ANIMATABLE, IDS_TCFLOWSPEEDV,
	p_default,	0.0f,
	p_range,	-32.0f, 32.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT5, IDC_SPIN2, 0.01f,
	end,
 
// 	eParamID_TcRotateSpeed,
	eParamID_TcRotateSpeed, _T("Tc Rotate Speed"), TYPE_FLOAT, P_ANIMATABLE, IDS_TCROTATESPEED,
	p_default,	0.0f,
	p_range,	-32.0f, 32.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT8, IDC_SPIN3, 0.01f,
	end,
// 	eParamID_Color,
	eParamID_Color, _T("Diffuse"), TYPE_TEXMAP, P_OWNERS_REF, IDS_COLOR,
	p_refno,		eMaxMaterialRef_Texmap,
	p_subtexno,		0,		
	p_accessor,	&tMaxPBAccessor,
	p_ui, TYPE_TEXMAPBUTTON, IDC_CHECK13,
	end,

// 	eParamID_UnnamedTransparency,
	eParamID_UnnamedTransparency, _T("Unnamed Transparency"), TYPE_FLOAT, P_ANIMATABLE, IDS_UNNAMEDTRANSPARENCY,
	p_default,	1.0f,
	p_range,	0.0f, 1.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT14, IDC_SPIN4, 0.01f,
	end,
// 	eParamID_UnnamedColor,
	eParamID_UnnamedColor, _T("Color"), TYPE_RGBA, P_ANIMATABLE, IDS_UNNAMEDCOLOR,
	p_accessor,	&tMaxPBAccessor,
	p_default,		Point3(1,1,1), 
	p_ui, TYPE_COLORSWATCH, IDC_DOSTUFF,
	end,
// 	eParamID_TextureRows,
	eParamID_TextureRows, _T("Texture Rows"), TYPE_INT, P_ANIMATABLE, IDS_TEXTUREROWS,
	p_default,	1,
	p_range,	1, 16,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_INT,	IDC_EDIT15, IDC_SPIN5, 1.0f,
	end,
// 	eParamID_TextureCols,
	eParamID_TextureCols, _T("Texture Cols"), TYPE_INT, P_ANIMATABLE, IDS_TEXTURECOLS,
	p_default,	1,
	p_range,	1, 16,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_INT,	IDC_EDIT16, IDC_SPIN6, 1.0f,
	end,
// 	eParamID_TextureChangeStyle,
	eParamID_TextureChangeStyle, _T("Textured Change Style"), TYPE_INT, 0, IDS_TEXTURECHANGESTYLE,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO2, eTextureChangeStyle_Size, IDS_TEXTURECHANGESTYLE_SEQUENCE, IDS_TEXTURECHANGESTYLE_RANDOM, IDS_TEXTURECHANGESTYLE_STOP,
	end,
// 	eParamID_TextureChangeInterval,
	eParamID_TextureChangeInterval, _T("Texture Change Interval"), TYPE_INT, P_ANIMATABLE, IDS_TEXTURECHANGEINTERVAL,
	p_default,	0,
	p_range,	1, 5000,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT17, IDC_SPIN7, 1.0f,
	end,
// 	eParamID_DontCastShadow,
	eParamID_DontCastShadow, _T("Dont Cast Shadow"), TYPE_BOOL, 0, IDS_DONTCASTSHADOW,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK7,
	end,
// 	eParamID_Unlit,
	eParamID_Unlit, _T("Unlit"), TYPE_BOOL, 0, IDS_UNLIT,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK8,
	end,
// 	eParamID_NoDepthCheck,
	eParamID_NoDepthCheck, _T("No Depth Check"), TYPE_BOOL, 0, IDS_NODEPTHCHECK,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK9,
	end,
// 	eParamID_NoDepthWrite,
	eParamID_NoDepthWrite, _T("No Depth Write"), TYPE_BOOL, 0, IDS_NODEPTHWRITE,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK10,
	end,
// 	eParamID_TexturedChannel,
	eParamID_TexturedChannel, _T("Textured Channel"), TYPE_INT, 0, IDS_TEXTUREDCHANNEL,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO3, eTexturedChannel_Size, IDS_TEXTUREDCHANNEL_NONE, IDS_TEXTUREDCHANNEL_COLOR, IDS_TEXTUREDCHANNEL_TRANSPARENCY,
																	IDS_TEXTUREDCHANNEL_INCADESCENCE, IDS_TEXTUREDCHANNEL_BUMPMAP, IDS_TEXTUREDCHANNEL_COMBINEDTEXTURES,	
	end,

// 	eParamID_TextureResolution,
	eParamID_TextureResolution, _T("Texture Resolution"), TYPE_INT, 0, IDS_TEXTURERESOLUTION,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO4, eTextureResolution_Size, IDS_TEXTURERESOLUTION_DEFAULT, IDS_TEXTURERESOLUTION_LOW, IDS_TEXTURERESOLUTION_MEDIUM,
																	  IDS_TEXTURERESOLUTION_HIGH,	 IDS_TEXTURERESOLUTION_HIGHEST,	
	end,
// 	eParamID_TextureFilter,
	eParamID_TextureFilter, _T("Texture Filter"), TYPE_INT, 0, IDS_TEXTUREFILTER,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO5, eTextureFilter_Size, IDS_TEXTUREFILTER_USEGLOBALSETTING, IDS_TEXTUREFILTER_NEAREST, IDS_TEXTUREFILTER_BILINEAR,
																  IDS_TEXTUREFILTER_MIPMAP_NEAREST,	IDS_TEXTUREFILTER_MIPMAP_LINEAR, IDS_TEXTUREFILTER_MIPMAP_BILINEAR,
																  IDS_TEXTUREFILTER_MIPMAP_TRILINEAR,
	end,
// 	eParamID_Caching,
	eParamID_Caching, _T("Caching"), TYPE_BOOL, 0, IDS_CACHING,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK11,
	end,
// 	eParamID_NodeState,
	eParamID_NodeState, _T("Node State"), TYPE_INT, 0, IDS_NODESTATE,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_INTLISTBOX, IDC_COMBO6, eNodeState_Size, IDS_NODESTATE_NORMAL, IDS_NODESTATE_HASNOEFFECT, IDS_NODESTATE_BLOCKING,
															  IDS_NODESTATE_WAITING_NORMAL,	IDS_NODESTATE_WAITING_HASNOEFFECT, IDS_NODESTATE_WAITING_BLOCKING,
	end,
// 	eParamID_AdditiveLayer,
	eParamID_AdditiveLayer, _T("Additive Layer"), TYPE_BOOL, 0, IDS_ADDITIVELAYER,
	p_default,	false,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SINGLECHEKBOX, IDC_CHECK12,
	end,
// 	eParamID_TranslucenceCoeff,
	eParamID_TranslucenceCoeff, _T("Translucence Coeff"), TYPE_FLOAT, P_ANIMATABLE, IDS_TRANSLUSCENCECOEFF,
	p_default,	0.0f,
	p_range,	0.0f, 1.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT18, IDC_SPIN8, 1.0f,
	end,
// 	eParamID_DiffuseReflectivity,
	eParamID_DiffuseReflectivity, _T("Diffuse Reflectivity"), TYPE_FLOAT, P_ANIMATABLE, IDS_DIFFUSEREFECTIVITY,
	p_default,	0.0f,
	p_range,	0.0f, 1.0f,
	p_accessor,	&tMaxPBAccessor,
	p_ui,		TYPE_SPINNER,	EDITTYPE_FLOAT,	IDC_EDIT19, IDC_SPIN9, 1.0f,
	end,
// 	eParamID_Incandescence,
	eParamID_Incandescence, _T("Incandescence"), TYPE_RGBA, P_ANIMATABLE, IDS_INCANDESCENCE,
	p_accessor,	&tMaxPBAccessor,
	p_default,		Point3(1,1,1), 
	p_ui, TYPE_COLORSWATCH, IDC_DOSTUFF2,
	end,
// 	eParamID_Transparency,
// 	eParamID_Mtl,
// 	eParamID_Mtl, _T("Material"), TYPE_MTL, P_OWNERS_REF, IDS_MATERIAL,
// 	p_refno,		eMaxMaterialRef_Mtl,
// 	p_submtlno,		0,		
// 	p_accessor,	&tMaxPBAccessor,
// 	p_ui, TYPE_MTLBUTTON, IDC_CHECK14,
// 	end,

	// end tag
	end
  );



#endif // __MaxParamBlockDesc2s_h__
