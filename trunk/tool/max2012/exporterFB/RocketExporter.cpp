#include "RocketCommon.h"
#include "RocketExporter.h"
#include "IMaxExporterFB.h"

RocketExporter::RocketExporter()
{
	//_exts.push_back("Mz");
	_exts.push_back("Fb");
}

RocketExporter::~RocketExporter()
{

}

int RocketExporter::ExtCount()
{
	return _exts.size();
}

const MCHAR * RocketExporter::Ext( int n )
{
	return _exts[n].c_str();
}

const MCHAR * RocketExporter::LongDesc()
{
	return tLongDesc.c_str();
}

const MCHAR * RocketExporter::ShortDesc()
{
	return tShortDesc.c_str();
}

const MCHAR * RocketExporter::AuthorName()
{
	return tAuthor.c_str();
}

const MCHAR * RocketExporter::CopyrightMessage()
{
	return tCopyRight.c_str();
}

const MCHAR * RocketExporter::OtherMessage1()
{
	return NULL;
}

const MCHAR * RocketExporter::OtherMessage2()
{
	return NULL;
}

unsigned int RocketExporter::Version()
{
	return tVersion;
}

void RocketExporter::ShowAbout( HWND hWnd )
{

}

int RocketExporter::DoExport( const MCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts/*=FALSE*/, DWORD options/*=0*/ )
{
	std::string fileName(name);
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), std::tolower);

// 	if (options & SCENE_EXPORT_SELECTED)
// 	{
// 		GetExportOptions()->exportAll = false;
// 	}
// 	else
// 	{
// 		GetExportOptions()->exportAll = true;
// 	}
// 
// 	if (options & SCENE_EXPORT_VERTEX_COLOR)
// 	{
// 		GetExportOptions()->exportVertexColour = true;
// 	}
// 	else
// 	{
// 		GetExportOptions()->exportVertexColour = false;
// 	}
	GetExportOptions()->m_outFilePath = fileName;
	_enum.setTime(i->GetTime());
	_enum.addNode(i->GetRootNode());

// 	if (fileName.compare(fileName.size() - 3, 3, ".mz") == 0)
// 	{
// 		// Prompt the user with our dialogbox, and get all the options.
		if (!suppressPrompts)
		{
			if (!DialogBoxParam(gInstance, MAKEINTRESOURCE(IDD_DIALOG1), i->GetMAXHWnd(), ExportOptionsDlgProc, (LPARAM)this)) 
			{
				return 1;
			}
		}
// 
// 		_enum.read();
// 		_enum.write();
// 		GetExportOptions()->exportMode = eExportMode_MZ;
// 	}
// 	else if (fileName.compare(fileName.size() - 3, 3, ".fb") == 0)
	{
		GetExportOptions()->exportMode = eExportMode_FB;
		//_enum.readBone();
		if (_enum.read())
		{
			_enum.writeBone();
		}
	}
// 	else
// 	{
// 		MessageBox(NULL, "无法识别的导出文件后缀", "Error", MB_OK);
// 		return 1;
// 	}

	return 1;
}

BOOL RocketExporter::SupportsOptions( int ext, DWORD options )
{
	return TRUE;
}

const SClass_ID RocketExporter::tSID(SCENE_EXPORT_CLASS_ID);
const Class_ID RocketExporter::tCID(tMaxExporterFBCID);
const int RocketExporter::tVersion(30);
const std::string RocketExporter::tName("Rocket ExporterFB");
const std::string RocketExporter::tLongDesc("Rocket 3D Max ExporterFB");
const std::string RocketExporter::tShortDesc("深圳冰川网络有限公司Exporter");
const std::string RocketExporter::tAuthor("cpzhang");
const std::string RocketExporter::tCopyRight("冰川网络公司");
const std::string RocketExporter::tCategory("Rocket");
