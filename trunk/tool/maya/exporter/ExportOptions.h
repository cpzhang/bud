#pragma once

typedef struct 
{
	int startFrame;							// ��ʼ֡
	int endFrame;							// ����֡
	int stepFrame;							// ����
	MString name;						// ��������
} clipInfo;

class ExportOptions
{
public:
	ExportOptions(void);
	static ExportOptions& ExportOptions::instance();

	BOOL parseExportOptions(const MFileObject& file,const MString& ExportOptionsString,bool  bExportSelected);
	std::string getOutFile() const	{ return m_outFile; }

public:
	std::string m_outFile;								// �����ļ����ļ���
	
public:
	bool exportAll;										// ����ȫ�����Ǳ�ѡ	��	
	bool exportMesh,exportVertexColour;					// ����mesh������������ɫ��
	bool exportMaterials,copyTextures;					// �������ʡ������ļ�����ǰ�ļ��У�
	bool exportSkeleton,exportAnims,exportAllFrame;		// ��������������������֡��
	
	MString m_outFilePath;								// �ļ�·��
	
	std::vector<clipInfo> clipList;						// �����б�
};

//#define ExportOptions ExportOptions::instance()
