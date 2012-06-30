#include "common.h"
#include ".\material.h"

material::material(void)
{
	clear();
}

material::~material(void)
{
	safeDelete(m_pShaderNode);
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
	m_pShaderNode = 0;

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

extern bool equal(float x,float y);
extern int Round (float fValue) ;
extern unsigned int getFps();

bool equal(colorNalpha x,colorNalpha y)
{
	return equal(x.r,y.r) && equal(x.g,y.g) && equal(x.b,y.b);
}

MStatus material::loadKeyframe(float time)
{
	uint fps = getFps();
	float frameTime = 1000.0f / fps;

	MStatus status;
	MPlugArray colorSrcPlugs;
	MPlug colorplug = m_pShaderNode->findPlug("color");
	if(colorplug.isNull())return MS::kFailure;

	colorplug.connectedTo(colorSrcPlugs,true,false);

	//透明度
	keyframeT<float> kf;
	kf.time = time;
	kf.data = 1.0f;

	MPlug plug = m_pShaderNode->findPlug("unnamedTransparency",&status);
	if(status)
	{
		plug.getValue(kf.data);
	}

	size_t size = m_vTransparencies.size();
	if(size > 0)
	{
		keyframeT<float>& t = m_vTransparencies[size - 1];
		if(!equal(t.data,kf.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<float> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vTransparencies.push_back(temp);
			}
			m_vTransparencies.push_back(kf);
		}
	}
	else
	{
		m_vTransparencies.push_back(kf);
	}

	//颜色
	keyframeT<colorNalpha> kfC;
	kfC.time = time;
	kfC.data = colorNalpha(1.0f,1.0f,1.0f);
	plug = m_pShaderNode->findPlug("unnamedColor",&status);
	if(status)
	{
		MObject object;
		plug.getValue(object);
		MFnNumericData data(object);
		colorNalpha color;
		data.getData(kfC.data.r,kfC.data.g,kfC.data.b);
	}
	size = m_vColors.size();
	if(size > 0)
	{
		keyframeT<colorNalpha>& t = m_vColors[size - 1];
		if(!equal(t.data,kfC.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<colorNalpha> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vColors.push_back(temp);
			}
			m_vColors.push_back(kfC);
		}
	}
	else
	{
		m_vColors.push_back(kfC);
	}

	//TcFlowSpeedU
	keyframeT<float> kfFlowSpeedU;
	kfFlowSpeedU.time = time;
	kfFlowSpeedU.data = false;

	plug = m_pShaderNode->findPlug("tcFlowSpeedU",&status);
	if(status)
	{
		plug.getValue(kfFlowSpeedU.data);
	}

	size = m_vTcFlowSpeedU.size();
	if(size > 0)
	{
		keyframeT<float>& t = m_vTcFlowSpeedU[size - 1];
		if(!equal(t.data,kfFlowSpeedU.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<float> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vTcFlowSpeedU.push_back(temp);
			}
			m_vTcFlowSpeedU.push_back(kfFlowSpeedU);
		}
	}
	else
	{
		m_vTcFlowSpeedU.push_back(kfFlowSpeedU);
	}

	//TcFlowV
	keyframeT<float> kfFlowSpeedV;
	kfFlowSpeedV.time = time;
	kfFlowSpeedV.data = false;

	plug = m_pShaderNode->findPlug("tcFlowSpeedV",&status);
	if(status)
	{
		plug.getValue(kfFlowSpeedV.data);
	}

	size = m_vTcFlowSpeedV.size();
	if(size > 0)
	{
		keyframeT<float>& t = m_vTcFlowSpeedV[size - 1];
		if(!equal(t.data,kfFlowSpeedV.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<float> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vTcFlowSpeedV.push_back(temp);
			}
			m_vTcFlowSpeedV.push_back(kfFlowSpeedV);
		}
	}
	else
	{
		m_vTcFlowSpeedV.push_back(kfFlowSpeedV);
	}

	//TcRotateSpeed
	keyframeT<float> kfRotateSpeed;
	kfRotateSpeed.time = time;
	kfRotateSpeed.data = false;

	plug = m_pShaderNode->findPlug("tcRotateSpeed",&status);
	if(status)
	{
		plug.getValue(kfRotateSpeed.data);
	}

	size = m_vRotateSpeed.size();
	if(size > 0)
	{
		keyframeT<float>& t = m_vRotateSpeed[size - 1];
		if(!equal(t.data,kfRotateSpeed.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<float> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vRotateSpeed.push_back(temp);
			}
			m_vRotateSpeed.push_back(kfRotateSpeed);
		}
	}
	else
	{
		m_vRotateSpeed.push_back(kfRotateSpeed);
	}

	//ChangeStyle
	keyframeT<short> kfChangeStyle;
	kfChangeStyle.time = time;
	kfChangeStyle.data = 0;	//Sequence

	plug = m_pShaderNode->findPlug("textureChangeStyle",&status);
	if(status)
	{
		plug.getValue(kfChangeStyle.data);
	}

	size = m_vChangeStyle.size();
	if(size > 0)
	{
		keyframeT<short>& t = m_vChangeStyle[size - 1];
		if(!equal(t.data,kfChangeStyle.data))
		{
			//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
			int lastTime = Round(t.time / frameTime);
			int thisTime = time / frameTime;
			if(thisTime - 1 > lastTime)
			{
				keyframeT<short> temp = t;
				temp.time = (time - 1) * frameTime;
				m_vChangeStyle.push_back(temp);
			}
			m_vChangeStyle.push_back(kfChangeStyle);
		}
	}
	else
	{
		m_vChangeStyle.push_back(kfChangeStyle);
	}

	return MS::kSuccess;
}

// Load material data 
MStatus material::load(MFnDependencyNode *pShaderNode,MStringArray& uvsets)
{	
	clear();

	MStatus stat;
	m_name = pShaderNode->name();

	m_pShaderNode = pShaderNode;

	MPlugArray colorSrcPlugs;
	pShaderNode->findPlug("color").connectedTo(colorSrcPlugs,true,false);
	if(colorSrcPlugs.length() <= 0)
	{
		//MessageBox(0,"materials must have MFn::kFileTexture connected to it",0,0);
		return MS::kFailure;
	}
	if (!colorSrcPlugs[0].node().hasFn(MFn::kFileTexture))
	{
		//MessageBox(0,"materials must have MFn::kFileTexture connected to it",0,0);
		return MS::kFailure;
	}
	MFnDependencyNode *pTextureNode = new MFnDependencyNode(colorSrcPlugs[0].node());

	bool tcFlowU = false;
	bool tcFlowV = false;
	bool tcRotate = false;
	bool tcAngleReset = false;
	bool tcPositionReset = false;
	bool twoSide = false;
	short bm = BM_OPAQUE;
	MStatus status;
	MPlug plug = pShaderNode->findPlug("blendMode",&status);
	if(status)
	{
		plug.getValue(bm);

		plug = pShaderNode->findPlug("twoSide",&status);
		if(status)
		{
			plug.getValue(twoSide);
		}

		plug = pShaderNode->findPlug("tcFlowU",&status);
		if(status)
		{
			plug.getValue(tcFlowU);
		}
		m_tcFlowU = tcFlowU;

		plug = pShaderNode->findPlug("tcFlowV",&status);
		if(status)
		{

			plug.getValue(tcFlowV);
		}
		m_tcFlowV = tcFlowV;

		//
		plug = pShaderNode->findPlug("tcRotate",&status);
		if(status)
		{
			plug.getValue(tcRotate);
		}
		m_tcRotate = tcRotate;

		//
		plug = pShaderNode->findPlug("tcAngleReset",&status);
		if(status)
		{
			plug.getValue(tcAngleReset);
		}
		m_tcAngleReset = tcAngleReset;

		//
		plug = pShaderNode->findPlug("tcPositionReset",&status);
		if(status)
		{
			plug.getValue(tcPositionReset);
		}
		m_tcPositionReset = tcPositionReset;

		plug = pShaderNode->findPlug("tcFlowSpeedU",&status);
		if(status)
		{
			plug.getValue(m_tcFlowSpeedU);
		}

		plug = pShaderNode->findPlug("tcFlowSpeedV",&status);
		if(status)
		{
			plug.getValue(m_tcFlowSpeedV);
		}

		//
		plug = pShaderNode->findPlug("tcRotateSpeed",&status);
		if(status)
		{
			plug.getValue(m_tcRotateSpeed);
		}

		plug = pShaderNode->findPlug("unnamedTransparency",&status);
		if(status)
		{
			plug.getValue(m_transparency);
		}

		plug = pShaderNode->findPlug("unnamedColor",&status);
		if(status)
		{
			MObject object;
			plug.getValue(object);
			MFnNumericData data(object);
			data.getData(m_color.r,m_color.g,m_color.b);
		}

		plug = pShaderNode->findPlug("textureRows",&status);
		if(status)
		{
			plug.getValue(m_rows);
		}

		plug = pShaderNode->findPlug("textureCols",&status);
		if(status)
		{
			plug.getValue(m_cols);
		}

		plug = pShaderNode->findPlug("textureChangeStyle",&status);
		if(status)
		{
			plug.getValue(m_changeStyle);
		}

		plug = pShaderNode->findPlug("textureChangeInterval",&status);
		if(status)
		{
			plug.getValue(m_changeInterval);
		}

		plug = pShaderNode->findPlug("dontCastShadow",&status);
		if(status)
		{
			plug.getValue(m_dontCastShadow);
		}
		plug = pShaderNode->findPlug("unlit",&status);
		if(status)
		{
			plug.getValue(m_unlit);
		}
		plug = pShaderNode->findPlug("noDepthCheck",&status);
		if(status)
		{
			plug.getValue(m_noDepthCheck);
		}
		plug = pShaderNode->findPlug("noDepthWrite",&status);
		if(status)
		{
			plug.getValue(m_noDepthWrite);
		}
		plug = pShaderNode->findPlug("additiveLayer",&status);
		if(status)
		{
			plug.getValue(m_additiveLayer);
		}
	}
	else
	{
		char name[256];
		strcpy(name,m_name.asChar());
		if(strlen(name) >= 6)
		{
			if(strstr(name,"sm_tm_") == name){m_twoSide = true;bm = BM_ALPHA_BLEND;}
			if(strstr(name,"sm_ck_") == name){m_twoSide = true;bm = BM_TRANSPARENT;}
		}
		if(strlen(name) >= 3)
		{
			if(strstr(name,"ck_") == name){bm = BM_TRANSPARENT;}
			if(strstr(name,"sm_") == name){m_twoSide = true;}
			if(strstr(name,"tm_") == name){bm = BM_ALPHA_BLEND;}
			if(strstr(name,"pt_") == name){}
		}
	}

	m_twoSide = twoSide;
	loadTexture(pTextureNode,(BlendModes&)bm,uvsets);

	delete pTextureNode;

	return MS::kSuccess;
}


// Load texture data from a texture node
MStatus material::loadTexture(MFnDependencyNode* pTexNode,BlendModes& blendModes,MStringArray& uvsets)
{
	texture tex;
	// Get texture filename
	MString filename, absFilename;
	MRenderUtil::exactFileTextureName(pTexNode->object(),absFilename);
	filename = absFilename.substring(absFilename.rindex('/')+1,absFilename.length()-1);
	MString command = "toNativePath(\"";
	command += absFilename;
	command += "\")";
	MGlobal::executeCommand(command,absFilename);
	tex.absFilename = absFilename;
	tex.filename = filename;
	tex.uvsetIndex = 0;
	tex.uvsetName = "";
	const char* tempChar = absFilename.asChar();
	// Set texture operation type
	tex.blendModes = blendModes;
	// Get connections to uvCoord attribute of texture node
	MPlugArray texSrcPlugs;
	pTexNode->findPlug("uvCoord").connectedTo(texSrcPlugs,true,false);
	// Get place2dtexture node (if connected)
	MFnDependencyNode* pPlace2dTexNode = NULL;
	for (int j=0; j<texSrcPlugs.length(); j++)
	{
		if (texSrcPlugs[j].node().hasFn(MFn::kPlace2dTexture))
		{
			pPlace2dTexNode = new MFnDependencyNode(texSrcPlugs[j].node());
			continue;
		}
	}
	// Get uvChooser node (if connected)
	MFnDependencyNode* pUvChooserNode = NULL;
	if (pPlace2dTexNode)
	{
		MPlugArray placetexSrcPlugs;
		pPlace2dTexNode->findPlug("uvCoord").connectedTo(placetexSrcPlugs,true,false);
		for (int j=0; j<placetexSrcPlugs.length(); j++)
		{
			if (placetexSrcPlugs[j].node().hasFn(MFn::kUvChooser))
			{
				pUvChooserNode = new MFnDependencyNode(placetexSrcPlugs[j].node());
				continue;
			}
		}
	}
	// Get uvset index
	if (pUvChooserNode)
	{
		bool foundMesh = false;
		bool foundUvset = false;
		MPlug uvsetsPlug = pUvChooserNode->findPlug("uvSets");
		MPlugArray uvsetsSrcPlugs;
		for (int i=0; i<uvsetsPlug.evaluateNumElements() && !foundMesh; i++)
		{
			uvsetsPlug[i].connectedTo(uvsetsSrcPlugs,true,false);
			for (int j=0; j<uvsetsSrcPlugs.length() && !foundMesh; j++)
			{
				if (uvsetsSrcPlugs[j].node().hasFn(MFn::kMesh))
				{
					uvsetsSrcPlugs[j].getValue(tex.uvsetName);
					for (int k=0; k<uvsets.length() && !foundUvset; k++)
					{
						if (uvsets[k] == tex.uvsetName)
						{
							tex.uvsetIndex = k;
							foundUvset = true;
						}
					}
				}
			}
		}
	}
	// add texture to material texture list
	m_textures.push_back(tex);
	// free up memory
	if (pUvChooserNode)
		delete pUvChooserNode;
	if (pPlace2dTexNode)
		delete pPlace2dTexNode;

	return MS::kSuccess;
}

// Copy textures to path specified by params
MStatus material::copyTextures()
{
	for (int i=0; i<m_textures.size(); i++)
	{
		// Copy file texture to output dir
		MString outFilename = ExportOptions::instance().m_outFilePath + "\\" + m_textures[i].filename;
		const char *s1 = m_textures[i].absFilename.asChar();
		const char *s2 = outFilename.asChar();

		BOOL overwrite = TRUE;
		/*if(rkt::isFile(s2))
		{
			char str[256];
			sprintf(str,"%s已经存在，要覆盖吗？",s2);
			overwrite = (MessageBox(0,str,"提问",MB_YESNO) == IDYES);
		}*/
		CopyFile(s1,s2,!overwrite);
	}
	return MS::kSuccess;
}