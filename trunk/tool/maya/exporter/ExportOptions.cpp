#include "common.h"
#include ".\ExportOptions.h"

ExportOptions::ExportOptions(void)
{
	exportAll = true;
	exportMesh = true;
	exportVertexColour = false;
	exportMaterials = true;
	copyTextures = true;
	exportSkeleton = true;
	exportAnims = true;
	exportAllFrame = true;

	m_outFilePath = "";
	clipList.clear();
}

ExportOptions& ExportOptions::instance()
{
	static ExportOptions ExportOptions;
	return ExportOptions;
}

BOOL ExportOptions::parseExportOptions(const MFileObject& file,const MString& ExportOptionsString,bool  bExportSelected)
{
	m_outFile = file.fullName().asChar();

	size_t iPos = m_outFile.find_last_of("/");
	std::string path = m_outFile.substr(0,iPos);
	m_outFilePath = path.c_str();

	exportAll = !bExportSelected;

	std::vector<MString> ActionNames;
	std::vector<int> startFrames;
	std::vector<int> endFrames;
	std::vector<int> stepFrames;
	clipList.clear();

	// 解析脚本
	MStringArray masOptionList;
	if (ExportOptionsString.length() <= 0)
		return false; 

	ExportOptionsString.split(';', masOptionList);

	for(int i = 0;i < (int)masOptionList.length();i++)
	{
		MStringArray masOption;

		masOptionList[i].split('=', masOption);

		if (1 >= masOption.length()) 
			continue;	

		if(masOption[0] == "exportMesh")
		{
			if(masOption[1] == "false")
			{
				exportMesh = false;
				exportVertexColour = false;
			}
			else
				exportMesh = true;
		}
		else if(masOption[0] == "exportVertexColour")
		{
			if(masOption[1] == "false")
				exportVertexColour = false;
			else
				exportVertexColour = true;
		}
		else if (masOption[0] == "exportMaterials") 
		{
			if(masOption[1] == "false")
			{
				exportMaterials = false;
				copyTextures = false;
			}
			else
				exportMaterials = true;
		}
		else if(masOption[0] == "copyTextures")
		{
			if(masOption[1] == "false")
				copyTextures = false;
			else
				copyTextures = true;
		}
		else if (masOption[0] == "exportSkeleton") 
		{
			if(masOption[1] == "false")
			{
				exportSkeleton = false;
				exportAnims = false;
				exportAllFrame = false;
			}
			else
				exportSkeleton = true;
		}
		else if (masOption[0] == "exportAnimation") 
		{
			if(masOption[1] == "false")
			{
				exportAnims = false;
				exportAllFrame = false;
			}
			else
				exportAnims = true;
		}
		else if (masOption[0] == "exportAllFrames") 
		{
			if(masOption[1] == "false")
				exportAllFrame = false;
			else
				exportAllFrame = true;

		}
		else if (masOption[0] == "actionName") 
		{
			MString sName = masOption[1];
			ActionNames.push_back(sName);
		}
		else if (masOption[0] == "startFrame") 
		{
			startFrames.push_back(masOption[1].asInt());
		}
		else if (masOption[0] == "endFrame") 
		{
			endFrames.push_back(masOption[1].asInt());
		}
		else if (masOption[0] == "stepFrame") 
		{
			stepFrames.push_back(masOption[1].asInt());
		}
	}

	for(int i = 0;i<ActionNames.size();i++)
	{	
		clipInfo clip;
		clip.name = ActionNames[i];
		clip.startFrame = startFrames[i];
		clip.endFrame = endFrames[i];
		clip.stepFrame = stepFrames[i];

		clipList.push_back(clip);
	}

	if(exportAllFrame)
	{
		clipList.clear();
		MTime kTimeMin   = MAnimControl::minTime();		//整个场景的起始帧
		MTime kTimeMax   = MAnimControl::maxTime();		//整个场景的结束帧

		clipInfo clip;
		clip.name = "Default";
		clip.startFrame = (int)kTimeMin.value();
		clip.endFrame = (int)kTimeMax.value();
		clip.stepFrame = 1;
		clipList.push_back(clip);	

		char str[256];
		wsprintf(str,"startFrame = %d,endFrame = %d,stepFrame = %d",clip.startFrame,clip.endFrame,clip.stepFrame);
		MGlobal::displayInfo(str);
	}

	return TRUE;
}

