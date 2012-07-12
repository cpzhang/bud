#pragma once
#include "RocketCommon.h"
typedef struct 
{
	int startFrame;							// 开始帧
	int endFrame;							// 结束帧
	int stepFrame;							// 步长
	std::string name;						// 动作名称
} clipInfo;

enum eExportMode
{
	eExportMode_MZ,
	eExportMode_FB,
	eExportMode_Size,
};

class ExportOptions
{
public:
	ExportOptions(void);

public:
	std::string m_outFile;								// 导出文件的文件名
	std::string m_outFolder;

	std::string getOutFolder()
	{
		size_t pos = m_outFilePath.find_last_of('\\');
		size_t num = pos + 1;
		m_outFolder = m_outFilePath.substr(0, num);
		
		return m_outFolder;
	}

	std::string getAnimationName()
	{
		std::string an("Default");
		size_t pos = m_outFilePath.find_last_of('_');
		if (pos != std::string::npos)
		{
			size_t num = m_outFilePath.size() - pos - 1;
			if (num > 0)
			{
				an = m_outFilePath.substr(pos + 1, num);
			}
		}
		
		return an;
	}
public:
	bool exportAll;										// 导出全部还是被选	？	
	bool exportMesh,exportVertexColour;					// 导出mesh、导出顶点颜色？
	bool exportMaterials,copyTextures;					// 导出材质、拷贝文件到当前文件夹？
	bool exportSkeleton,exportAnims,exportAllFrame;		// 导出骨骼、动画、所有帧？
	
	std::string m_outFilePath;							// 文件路径
	
	std::vector<clipInfo> clipList;						// 动画列表

	int frameIndex;
	eExportMode	exportMode;
	bool	deleteUnReferrencedBones;
};

static ExportOptions tExportOptions;

ExportOptions* GetExportOptions();
//#define ExportOptions ExportOptions::instance()
