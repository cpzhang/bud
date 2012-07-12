#pragma once
#include "ExportOptions.h"

extern bool IsRealEqual(const float& a, const float& b);
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
	colorNalpha operator = (const RGBA& k)
	{
		r = k.r;
		g = k.g;
		b = k.b;
	}
	colorNalpha &operator =(const colorNalpha &c) {  r=c.r; g=c.g; b=c.b; return *this; }

	float r,g,b;

	inline bool operator != (colorNalpha& k)
	{
		return (r != k.r || g != k.g || b != k.b);
	}

	inline bool operator == (const colorNalpha& rk) const
	{
		return (IsRealEqual(r, rk.r) && IsRealEqual(g, rk.g) && IsRealEqual(b, rk.b));
	}

	inline colorNalpha linear(float f, const colorNalpha& rk) const
	{
		colorNalpha s;
		s.r = f * r + (1 - f) * rk.r;
		s.g = f * g + (1 - f) * rk.g;
		s.b = f * b + (1 - f) * rk.b;

		return s;
	}
	colorNalpha operator *(float f)
	{
		return colorNalpha(f * r, f * g, f * b);
	}
	colorNalpha operator +(const colorNalpha& rk)
	{
		return colorNalpha(rk.r + r, rk.g + g, rk.b + b);
	}
	friend colorNalpha operator *(float f, const colorNalpha& rk)
	{
		return colorNalpha(f * rk.r, f * rk.g, f * rk.b);
	}
};

typedef struct textureTag
{
	textureTag()
	{
		blendModes = BM_OPAQUE;
	}
	std::string filename;
	std::string absFilename;
	BlendModes blendModes;
	std::string uvsetName;
	int uvsetIndex;
} texture;

class material
{
public:
	material(void);
	virtual ~material(void);

	void clear();
//	bool load(MFnDependencyNode *pShaderNode,std::stringArray& uvsets);
	bool loadKeyframe(float time);
	bool copyTextures();
public:
//	bool loadTexture(MFnDependencyNode* pTexNode,BlendModes& opType,std::stringArray& uvsets);

	std::string m_name;
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

	void sampleLinear()
	{
		SampleLinear(m_vTransparencies, 1);
		SampleLinear(m_vTcFlowSpeedU, 1);
		SampleLinear(m_vTcFlowSpeedV, 1);
		SampleLinear(m_vRotateSpeed, 1);
		SampleLinear(m_vColors, 1);
		SampleLinear(m_vChangeStyle, 1);
	}

	void processStillSequence()
	{
		if (CheckAllKeyFramesTheSame(m_vTransparencies))
		{
			m_vTransparencies.clear();
		}
		if (CheckAllKeyFramesTheSame(m_vTcFlowSpeedU))
		{
			m_vTcFlowSpeedU.clear();
		}
		if (CheckAllKeyFramesTheSame(m_vTcFlowSpeedV))
		{
			m_vTcFlowSpeedV.clear();
		}
		if (CheckAllKeyFramesTheSame(m_vRotateSpeed))
		{
			m_vRotateSpeed.clear();
		}
		if (CheckAllKeyFramesTheSame(m_vColors))
		{
			m_vColors.clear();
		}
		if (CheckAllKeyFramesTheSame(m_vChangeStyle))
		{
			m_vChangeStyle.clear();
		}
	}
};
