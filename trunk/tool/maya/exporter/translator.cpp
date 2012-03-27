#include "common.h"
#include ".\translator.h"

translator::~translator(void)
{
}

void* translator::creator()
{
	return new translator();
}


MStatus	translator::reader(const MFileObject& file, const MString& ExportOptionsString, MPxFileTranslator::FileAccessMode mode)
{ 
	return MS::kSuccess; 
};

bool translator::haveReadMethod() const
{ 
	return false; 
};

bool translator::haveWriteMethod() const
{ 
	return true; 
};

MString translator::defaultExtension() const	
{ 
	return "Mz"; 
}

bool translator::canBeOpened() const 
{ 
	return false;
}
MString translator::filter() const				
{ 
	return "*.Mz"; 
}

MPxFileTranslator::MFileKind translator::identifyFile(  const MFileObject& fileName,
														 const char* buffer, short size) const
{ 
	LPSTR extension = PathFindExtensionA(fileName.name().asChar());
	if(0==stricmp(extension, ".Mz"))
	{
		return kIsMyFileType;
	}
	else
	{
		return kNotMyFileType; 
	}
}


MStatus	translator::writer(const MFileObject& mfoMMFile,			// save file object
							  const MString& msExportOptions,					// ExportOptions::instance() string
							  MPxFileTranslator::FileAccessMode mode)		// ExportOptions::instance() string
{	
	//导出数据
	MStatus stat = MS::kSuccess;
	stat = ExportScene(mfoMMFile, msExportOptions, mode);
	if(!stat)
	{
		MGlobal::displayInfo("Failed in Export Scene.");
	}
	return stat;
}

#include "windowsx.h"
#include "resource.h"
bool g_bExportBones = false;
bool g_bExportVertexColor = false;

LRESULT CALLBACK OptionsFunc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			Button_SetCheck(GetDlgItem(hWnd,IDC_EXPORT_BONES),g_bExportBones);
			Button_SetCheck(GetDlgItem(hWnd,IDC_EXPORT_VERTEXCOLOR),g_bExportVertexColor);
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				g_bExportBones = Button_GetCheck(GetDlgItem(hWnd,IDC_EXPORT_BONES));
				g_bExportVertexColor = Button_GetCheck(GetDlgItem(hWnd,IDC_EXPORT_VERTEXCOLOR));
				EndDialog(hWnd,IDOK);
				return TRUE;
			case IDCANCEL:
				EndDialog(hWnd,IDCANCEL);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//#include "bindPoseTool.h"
MStatus	translator::ExportScene(const MFileObject& mfoMMFile,		// save file object
								   const MString& msExportOptions,					// ExportOptions::instance() string
								   MPxFileTranslator::FileAccessMode mode)
{
	//单实例先清除
	if(MaterialSet::getSingletonPtr())
		MaterialSet::getSingletonPtr()->clear();

	const char *opt = msExportOptions.asChar();
	MStatus stat = MS::kSuccess;
	MString	msFile= mfoMMFile.fullName();
	const char * file = msFile.asChar();
	CStringA filename(file);
	const char* pExtension= strrchr(file, '.');
	if(!pExtension)
	{
		filename.Append( ".Mz");

		if(PathFileExistsA( filename.GetString() ))
		{
			CStringA Msg("Would you like to write over the existing file?\n");
			Msg.Append( filename );

			if( IDYES != MessageBoxA(NULL, Msg.GetString(), "File Exists", MB_YESNOCANCEL ) )
			{
				return stat;
			}
		}
	} 
	else if(0 != _stricoll(".Mz", pExtension) )
	{
		MessageBoxA(NULL, "Cannot write over non- \".Mz\" file", "Incompatible File Type", 0 );
		return stat;
	}

	bool bExportSelected = (mode == MPxFileTranslator::kExportActiveAccessMode);
	
	// 设置输出个性化选项
	ExportOptions::instance().parseExportOptions(mfoMMFile,msExportOptions,bExportSelected);

	//int id = MessageBox(0,"导出动画数据吗？\n如果是导出静态模型，请选择“否”","提示",MB_YESNOCANCEL);
	//if(id == IDCANCEL)return MS::kFailure;
	//if(id == IDNO)ExportOptions::instance().exportAnims = false;
	if(DialogBox(GetModuleHandle("MayaExport_rd.mll"),MAKEINTRESOURCE(IDD_OPTIONS),0,(DLGPROC)OptionsFunc) == IDCANCEL)return MS::kFailure;
	ExportOptions::instance().exportAnims = g_bExportBones;
	ExportOptions::instance().exportVertexColour = g_bExportVertexColor;

	/*BindPoseTool bindPoseTool;
	bindPoseTool.GoIntoBindPose();*/
	m_mWriter.read();
	m_mWriter.write();
	/*bindPoseTool.UndoGoIntoBindPose();*/

	return stat;
}

