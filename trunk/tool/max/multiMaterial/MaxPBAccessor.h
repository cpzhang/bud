/********************************************************************
**	file: 		MaxPBAccessor.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __MaxPBAccessor_h__
#define __MaxPBAccessor_h__

#include "MaxCommon.h"
#include "MaxMaterial.h"
#include "MaxParamBlockDesc2s.h"

class MaxPBAccessor : public PBAccessor
{
public:
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
		MaxMaterial* m = (MaxMaterial*)owner;
		switch (id)
		{
			// use item data to unscramble sorted lists
		case eParamID_TwoSides: 
			{
				m->_isTwoSides = v.i;
				m->invalidateUI(eParamID_TwoSides);
			} break;
		case eParamID_BlendMode: 
			{
				m->_blendMode = (eBlendMode)v.i;
				m->invalidateUI(eParamID_BlendMode);
			} break;
		case eParamID_TcFlowU: 
			{
				m->_isTcFlowU = v.i;
				m->invalidateUI(eParamID_TcFlowU);
			} break;
		case eParamID_TcFlowV: 
			{
				m->_isTcFlowV = v.i;
				m->invalidateUI(eParamID_TcFlowV);
			} break;
		case eParamID_TcRotate: 
			{
				m->_isTcRotate = v.i;
				m->invalidateUI(eParamID_TcRotate);
			} break;
		case eParamID_TcAngleReset: 
			{
				m->_isTcAngleReset = v.i;
				m->invalidateUI(eParamID_TcAngleReset);
			} break;
		case eParamID_TcPositionReset: 
			{
				m->_isTcPositionReset = v.i;
				m->invalidateUI(eParamID_TcPositionReset);
			} break;
		case eParamID_TcFlowSpeedU: 
			{
				m->_tcFlowSpeedU = v.f;
				m->invalidateUI(eParamID_TcFlowSpeedU);
			} break;
		case eParamID_TcFlowSpeedV: 
			{
				m->_tcFlowSpeedV = v.f;
				m->invalidateUI(eParamID_TcFlowSpeedV);
			} break;
		case eParamID_TcRotateSpeed: 
			{
				m->_tcRotateSpeed = v.f;
				m->invalidateUI(eParamID_TcRotateSpeed);
			} break;
		case eParamID_Color: 
			{
			} break;
		case eParamID_UnnamedTransparency: 
			{
				m->_unnamedTransparency = v.f;
				m->invalidateUI(eParamID_UnnamedTransparency);
			} break;
		case eParamID_UnnamedColor: 
			{
				m->_color[0] = v.p->x;
				m->_color[1] = v.p->y;
				m->_color[2] = v.p->z;
				//m->_color.a = v.p4->w;
				m->invalidateUI(eParamID_UnnamedColor);
			} break;
		case eParamID_TextureRows: 
			{
				m->_textureRows = v.i;
				m->invalidateUI(eParamID_TextureRows);
			} break;
		case eParamID_TextureCols: 
			{
				m->_textureCols = v.i;
				m->invalidateUI(eParamID_TextureCols);
			} break;
		case eParamID_TextureChangeStyle: 
			{
				m->_textureChangeStyle = (eTextureChangeStyle)v.i;
				m->invalidateUI(eParamID_TextureChangeStyle);
			} break;
		case eParamID_TextureChangeInterval: 
			{
				m->_textureChangeInterval = v.f;
				m->invalidateUI(eParamID_TextureChangeInterval);
			} break;
		case eParamID_DontCastShadow: 
			{
				m->_dontCastShadow = v.i;
				m->invalidateUI(eParamID_DontCastShadow);
			} break;
		case eParamID_Unlit: 
			{
				m->_unLit = v.i;
				m->invalidateUI(eParamID_Unlit);
			} break;
		case eParamID_NoDepthCheck: 
			{
				m->_noDepthCheck = v.i;
				m->invalidateUI(eParamID_NoDepthCheck);
			} break;
		case eParamID_NoDepthWrite: 
			{
				m->_noDepthWrite = v.i;
				m->invalidateUI(eParamID_NoDepthWrite);
			} break;
		case eParamID_TexturedChannel: 
			{
				m->_textureChannel = (eTexturedChannel)v.i;
				m->invalidateUI(eParamID_TexturedChannel);
			} break;
		case eParamID_TextureResolution: 
			{
				m->_textureResolution = (eTextureResolution)v.i;
				m->invalidateUI(eParamID_TextureResolution);
			} break;
		case eParamID_TextureFilter: 
			{
				m->_textureFilter = (eTextureFilter)v.i;
				m->invalidateUI(eParamID_TextureFilter);
			} break;
		case eParamID_Caching: 
			{
				m->_caching = v.i;
				m->invalidateUI(eParamID_Caching);
			} break;
		case eParamID_NodeState: 
			{
				m->_nodeState = (eNodeState)v.i;
				m->invalidateUI(eParamID_NodeState);
			} break;
		case eParamID_AdditiveLayer: 
			{
				m->_additiveLayer = v.i;
				m->invalidateUI(eParamID_AdditiveLayer);
			} break;
		case eParamID_TranslucenceCoeff: 
			{
				m->_translucenceCoeff = v.f;
				m->invalidateUI(eParamID_TranslucenceCoeff);
			} break;
		case eParamID_DiffuseReflectivity: 
			{
				m->_diffuseReflectivity = v.f;
				m->invalidateUI(eParamID_DiffuseReflectivity);
			} break;
		case eParamID_Incandescence: 
			{
				m->_incandescence = *v.p;
				m->invalidateUI(eParamID_Incandescence);
			} break;
		case eParamID_Transparency: 
			{
			} break;
		default:
			break;
		}
	}

	void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval& valid)    // get into v
	{
		return;
		MaxMaterial* m = (MaxMaterial*)owner;
		switch (id)
		{
		case eParamID_TwoSides: 
			{
				v.i = m->_isTwoSides;
			} break;
		case eParamID_BlendMode: 
			{
				v.i = m->_blendMode;
			} break;
		case eParamID_TcFlowU: 
			{
				v.i = m->_isTcFlowU;
			} break;
		case eParamID_TcFlowV: 
			{
				v.i = m->_isTcFlowV;
			} break;
		case eParamID_TcRotate: 
			{
				v.i = m->_isTcRotate;
			} break;
		case eParamID_TcAngleReset: 
			{
				v.i = m->_isTcAngleReset;
			} break;
		case eParamID_TcPositionReset: 
			{
				v.i = m->_isTcPositionReset;
			} break;
		case eParamID_TcFlowSpeedU: 
			{
				v.f = m->_tcFlowSpeedU;
			} break;
		case eParamID_TcFlowSpeedV: 
			{
				v.f = m->_tcFlowSpeedV;
			} break;
		case eParamID_TcRotateSpeed: 
			{
				v.f = m->_tcRotateSpeed;
			} break;
		case eParamID_Color: 
			{
			} break;
		case eParamID_UnnamedTransparency: 
			{
				 v.f = m->_unnamedTransparency;
			} break;
		case eParamID_UnnamedColor: 
			{
				*v.p = m->_color;
			} break;
		case eParamID_TextureRows: 
			{
				 v.i = m->_textureRows;
			} break;
		case eParamID_TextureCols: 
			{
				 v.i = m->_textureCols;
			} break;
		case eParamID_TextureChangeStyle: 
			{
				 v.i = m->_textureChangeStyle;
			} break;
		case eParamID_TextureChangeInterval: 
			{
				 v.f = m->_textureChangeInterval;
			} break;
		case eParamID_DontCastShadow: 
			{
				 v.i = m->_dontCastShadow;
			} break;
		case eParamID_Unlit: 
			{
				 v.i = m->_unLit;
			} break;
		case eParamID_NoDepthCheck: 
			{
				 v.i = m->_noDepthCheck;
			} break;
		case eParamID_NoDepthWrite: 
			{
				 v.i = m->_noDepthWrite;
			} break;
		case eParamID_TexturedChannel: 
			{
				v.i = m->_textureChannel;
			} break;
		case eParamID_TextureResolution: 
			{
				v.i = m->_textureResolution;
			} break;
		case eParamID_TextureFilter: 
			{
				v.i = m->_textureFilter;
			} break;
		case eParamID_Caching: 
			{
				 v.i = m->_caching;
			} break;
		case eParamID_NodeState: 
			{
				v.i = m->_nodeState;
			} break;
		case eParamID_AdditiveLayer: 
			{
				v.i = m->_additiveLayer;
			} break;
		case eParamID_TranslucenceCoeff: 
			{
				 v.f = m->_translucenceCoeff;
			} break;
		case eParamID_DiffuseReflectivity: 
			{
				 v.f = m->_diffuseReflectivity;
			} break;
		case eParamID_Incandescence: 
			{
				*v.p = m->_incandescence;
			} break;
		case eParamID_Transparency: 
			{
			} break;
		default:
			break;
		}
	}
};

static MaxPBAccessor tMaxPBAccessor;

#endif // __MaxPBAccessor_h__
