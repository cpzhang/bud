#include "PlatoCommon.h"
#include "PlatoExporter.h"
#include "IMaxExporter.h"

PlatoExporter::PlatoExporter()
{
	_exts.push_back("mesh");
}

PlatoExporter::~PlatoExporter()
{

}

int PlatoExporter::ExtCount()
{
	return _exts.size();
}

const MCHAR * PlatoExporter::Ext( int n )
{
	return _exts[n].c_str();
}

const MCHAR * PlatoExporter::LongDesc()
{
	return tLongDesc.c_str();
}

const MCHAR * PlatoExporter::ShortDesc()
{
	return tShortDesc.c_str();
}

const MCHAR * PlatoExporter::AuthorName()
{
	return tAuthor.c_str();
}

const MCHAR * PlatoExporter::CopyrightMessage()
{
	return tCopyRight.c_str();
}

const MCHAR * PlatoExporter::OtherMessage1()
{
	return NULL;
}

const MCHAR * PlatoExporter::OtherMessage2()
{
	return NULL;
}

unsigned int PlatoExporter::Version()
{
	return tVersion;
}

void PlatoExporter::ShowAbout( HWND hWnd )
{

}

int PlatoExporter::DoExport( const MCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts/*=FALSE*/, DWORD options/*=0*/ )
{
	std::string fileName(name);
	std::transform(fileName.begin(), fileName.end(), fileName.begin(), std::tolower);

	if (options & SCENE_EXPORT_SELECTED)
	{
		GetExportOptions()->exportAll = false;
	}
	else
	{
		GetExportOptions()->exportAll = true;
	}

	GetExportOptions()->m_outFilePath = fileName;
	_enum.setTime(i->GetTime());
	_enum.addNode(i->GetRootNode());

	{
		GetExportOptions()->exportMode = eExportMode_Terrain;
		if (_enum.readTerrain())
		{
			_enum.writeTerrain();
		}
	}
	return 1;
}

BOOL PlatoExporter::SupportsOptions( int ext, DWORD options )
{
	return TRUE;
}

const SClass_ID PlatoExporter::tSID(SCENE_EXPORT_CLASS_ID);
const Class_ID PlatoExporter::tCID(tMaxExporterCID);
const int PlatoExporter::tVersion(1);
const std::string PlatoExporter::tName("Plato Exporter of Mesh");
const std::string PlatoExporter::tLongDesc("Plato 3D Max Exporter of Mesh");
const std::string PlatoExporter::tShortDesc("Plato Exporter of Mesh");
const std::string PlatoExporter::tAuthor("cpzhang");
const std::string PlatoExporter::tCopyRight("2012 Plato");
const std::string PlatoExporter::tCategory("Plato");
