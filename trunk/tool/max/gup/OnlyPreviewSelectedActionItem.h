/********************************************************************
**	file: 		ParticlePreviewActionItem.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-27
**	purpose:	
*********************************************************************/
#ifndef __OnlyPreviewSelectedActionItem_h__
#define __OnlyPreviewSelectedActionItem_h__

#include "MaxGUPCommon.h"
#include "IMaxExporter.h"

class OnlyPreviewSelectedActionItem : public ActionItem
{
public:

	//ActionItem methods
	BOOL IsChecked()			
	{
		return FALSE;
	}
	void GetMenuText(TSTR& menuText)	
	{
		menuText = "Preview( + Vertex Color)";
	}

	void GetButtonText(TSTR& buttonText)
	{
		buttonText = "zButtonText";
	}

	void GetCategoryText(TSTR& catText)		
	{
		catText = "All Commands";
	}

	void GetDescriptionText(TSTR& descText)
	{
		descText = "Preview + Vertex Color";
	}

	BOOL ExecuteAction()			
	{
		char tempPath[MAX_PATH];
		GetTempPath(MAX_PATH, tempPath);
		std::string path(tempPath);
		path += "max.mz";

		//
		Class_ID cid = tMaxExporterCID;
		DWORD option = 0;
		//option |= SCENE_EXPORT_SELECTED;
		option |= SCENE_EXPORT_VERTEX_COLOR;
		option |= SCENE_EXPORT_ANIMATION_ALL;
		GetCOREInterface()->ExportToFile(path.c_str(), TRUE, option, &cid);

		//
		HWND hWnd = FindWindowEx(0,0,0,"MaxPreview");
		if(!hWnd)
		{
			char path[257];
			static const std::string tMaxProgramName("3dsmax.exe");
			GetModuleFileName(GetModuleHandle(tMaxProgramName.c_str()), path, 256);
			std::string parentPath(path);
			parentPath.erase(parentPath.size() - tMaxProgramName.size(), tMaxProgramName.size());
			std::string previewProgramPath(parentPath + "preview\\MaxPreview.exe");
			//
			if(!ShellExecute(0, "open", previewProgramPath.c_str(), "", "", SW_SHOW))
			{
				MessageBox(0, previewProgramPath.c_str(),"Can't Find MaxPreview Program", 0);
				return FALSE;
			}

			//
			hWnd = FindWindowEx(0,0,0,"MaxPreview");
			DWORD tick = GetTickCount();
			while(!hWnd)
			{
				DWORD tickNow = GetTickCount();
				if(tickNow - tick > 3000)break;
				Sleep(1);
				hWnd = FindWindowEx(0,0,0,"MaxPreview");
			}
		}

		if(hWnd)
		{
			SendMessage(hWnd, WM_USER + 128, 0, 0);

			//
			SetActiveWindow(hWnd);
			SetForegroundWindow(hWnd);
			BringWindowToTop(hWnd);
		}

		return FALSE;
	}

	int GetId()				
	{
		return _id;
	}

	BOOL IsItemVisible()
	{
		return TRUE;
	}

	BOOL IsEnabled()		
	{
		return TRUE;
	}

	MaxIcon* GetIcon() 
	{
		return NULL;
	}

	void DeleteThis()
	{
		delete this;
	}

	int _id;
};

#endif // __OnlyPreviewSelectedActionItem_h__
