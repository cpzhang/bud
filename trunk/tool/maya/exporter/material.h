#pragma once
#include "ExportOptions.h"

enum BlendModes
{
	BM_OPAQUE,
	BM_TRANSPARENT,
	BM_ALPHA_BLEND,
	BM_ADDITIVE,
	BM_ADDITIVE_ALPHA,
	BM_MODULATE
};

typedef enum MaterialTypeTag {MT_LAMBERT,MT_PHONG,MT_BLINN} MaterialType;

struct color
{
	color(){r=g=b=a=1.0f;}
	color(float ca,float cr,float cg,float cb){ a = ca; r = cr; g = cg; b = cb;}

	color &operator =(const color &c) {  r=c.r; g=c.g; b=c.b; a=c.a; return *this; 	}

	float a,r,g,b;
};

struct colorNalpha
{
	colorNalpha(){r=g=b=1.0f;}
	colorNalpha(float cr,float cg,float cb){ r = cr; g = cg; b = cb;}

	colorNalpha &operator =(const colorNalpha &c) {  r=c.r; g=c.g; b=c.b; return *this; }

	float r,g,b;
};

typedef struct textureTag
{
	MString filename;
	MString absFilename;
	BlendModes blendModes;
	MString uvsetName;
	int uvsetIndex;
} texture;

class material
{
public:
	material(void);
	virtual ~material(void);

	void clear();
	MStatus load(MFnDependencyNode *pShaderNode,MStringArray& uvsets);
	MStatus loadKeyframe(float time);
	MStatus copyTextures();
public:
	MStatus loadTexture(MFnDependencyNode* pTexNode,BlendModes& opType,MStringArray& uvsets);

	MString m_name;
	MaterialType m_type;
	color m_ambient, m_diffuse, m_specular, m_emissive;
	bool m_lightingOff;
	bool m_isTextured;
	bool m_isMultiTextured;
	bool m_twoSide;
	bool m_tcFlowU,m_tcFlowV,m_tcRotate, m_tcAngleReset, m_tcPositionReset;
	float m_tcFlowSpeedU,m_tcFlowSpeedV, m_tcRotateSpeed;
	float m_transparency;
	std::vector<keyframeT<float> > m_vTransparencies;
	std::vector<keyframeT<float> > m_vTcFlowSpeedU;	//版本号26加入关键桢
	std::vector<keyframeT<float> > m_vTcFlowSpeedV;	//版本号26加入关键桢
	std::vector<keyframeT<float> > m_vRotateSpeed;	//版本号28加入关键桢
	colorNalpha m_color;
	std::vector<keyframeT<colorNalpha> > m_vColors;
	std::vector<texture> m_textures;

	MFnDependencyNode*	m_pShaderNode;

	//序列贴图
	short m_rows;
	short m_cols;
	short m_changeStyle;//0 - Sequence 1 - Random
	short m_changeInterval;
	std::vector<keyframeT<short> > m_vChangeStyle;	//版本号26加入关键桢
	bool m_dontCastShadow;
	bool m_unlit;
	bool m_noDepthWrite;
	bool m_noDepthCheck;
	bool m_additiveLayer;	//版本号27加入叠加层次
};
