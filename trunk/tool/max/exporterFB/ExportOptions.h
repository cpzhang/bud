#pragma once
#include "RocketCommon.h"
typedef struct 
{
	int startFrame;							// ��ʼ֡
	int endFrame;							// ����֡
	int stepFrame;							// ����
	std::string name;						// ��������
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
	std::string m_outFile;								// �����ļ����ļ���
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
	bool exportAll;										// ����ȫ�����Ǳ�ѡ	��	
	bool exportMesh,exportVertexColour;					// ����mesh������������ɫ��
	bool exportMaterials,copyTextures;					// �������ʡ������ļ�����ǰ�ļ��У�
	bool exportSkeleton,exportAnims,exportAllFrame;		// ��������������������֡��
	
	std::string m_outFilePath;							// �ļ�·��
	
	std::vector<clipInfo> clipList;						// �����б�

	int frameIndex;
	eExportMode	exportMode;
	bool	deleteUnReferrencedBones;
};

static ExportOptions tExportOptions;

ExportOptions* GetExportOptions();
//#define ExportOptions ExportOptions::instance()
