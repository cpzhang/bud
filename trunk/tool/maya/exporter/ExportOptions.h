#pragma once

typedef struct 
{
	int startFrame;							// 开始帧
	int endFrame;							// 结束帧
	int stepFrame;							// 步长
	MString name;						// 动作名称
} clipInfo;

class ExportOptions
{
public:
	ExportOptions(void);
	static ExportOptions& ExportOptions::instance();

	BOOL parseExportOptions(const MFileObject& file,const MString& ExportOptionsString,bool  bExportSelected);
	std::string getOutFile() const	{ return m_outFile; }

public:
	std::string m_outFile;								// 导出文件的文件名
	
public:
	bool exportAll;										// 导出全部还是被选	？	
	bool exportMesh,exportVertexColour;					// 导出mesh、导出顶点颜色？
	bool exportMaterials,copyTextures;					// 导出材质、拷贝文件到当前文件夹？
	bool exportSkeleton,exportAnims,exportAllFrame;		// 导出骨骼、动画、所有帧？
	
	MString m_outFilePath;								// 文件路径
	
	std::vector<clipInfo> clipList;						// 动画列表
};

//#define ExportOptions ExportOptions::instance()
