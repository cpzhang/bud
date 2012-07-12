#include "material.h"

material::material(void)
{
	clear();
}

material::~material(void)
{
}

void material::clear()
{
	m_name = "";
	m_type = MT_LAMBERT;
	m_lightingOff = false;
	m_unlit = false;
	m_noDepthWrite = false;
	m_noDepthCheck = false;
	m_isTextured = false;
	m_isMultiTextured = false;
	m_ambient = color(0,0,0,0);
	m_diffuse = color(0,0,0,0);
	m_specular = color(0,0,0,0);
	m_emissive = color(0,0,0,0);
	m_textures.clear();
	m_twoSide = false;
	m_tcFlowU = false;
	m_tcFlowV = false;
	m_tcRotate = false;
	m_tcAngleReset = false;
	m_tcPositionReset = false;
	m_tcFlowSpeedU = 0.0f;
	m_tcFlowSpeedV = 0.0f;
	m_tcRotateSpeed = 0.0f;
	m_transparency = 1.0f;

	m_rows = 1;
	m_cols = 1;
	m_changeStyle = 0;
	m_changeInterval = 30;
	m_dontCastShadow = false;

	m_color = colorNalpha(1.0f,1.0f,1.0f);
	m_vTransparencies.clear();
	m_vColors.clear();

	m_additiveLayer = false;
}

// extern bool equal(float x,float y);
// extern int Round (float fValue) ;
// extern unsigned int getFps();
// 
// bool equal(colorNalpha x,colorNalpha y)
// {
// 	return equal(x.r,y.r) && equal(x.g,y.g) && equal(x.b,y.b);
// }

// Copy textures to path specified by params
bool material::copyTextures()
{
	std::string folder = GetExportOptions()->getOutFolder();

	uint nTextures = m_textures.size();
	for(int j= 0;j < nTextures;j++)
	{	
		texture tex = m_textures[j];
		std::string strFilename = tex.absFilename;	
		std::string strDest = folder + tex.filename;
		CopyFile(strFilename.c_str(), strDest.c_str(), false);
		//MessageBox(NULL, strFilename.c_str(), strDest.c_str(), MB_OK);
	}

	return true;
}