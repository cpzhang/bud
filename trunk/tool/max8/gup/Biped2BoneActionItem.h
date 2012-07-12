/********************************************************************
**	file: 		Biped2BoneActionItem.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-27
**	purpose:	
*********************************************************************/
#ifndef __Biped2BoneActionItem_h__
#define __Biped2BoneActionItem_h__

#include "MaxGUPCommon.h"
#include "IMaxExporter.h"

class Biped2BoneActionItem : public ActionItem
{
public:

	//ActionItem methods
	BOOL IsChecked()			
	{
		return FALSE;
	}
	void GetMenuText(TSTR& menuText)	
	{
		menuText = "Convert Biped to Bone";
	}

	void GetButtonText(TSTR& buttonText)
	{
		buttonText = "Biped2Bone";
	}

	void GetCategoryText(TSTR& catText)		
	{
		catText = "All Commands";
	}

	void GetDescriptionText(TSTR& descText)
	{
		descText = "Biped2Bone";
	}

	BOOL ExecuteAction()			
	{
		std::string path;
#if 0
		char tempPath[MAX_PATH];
		GetTempPath(MAX_PATH, tempPath);
		path = tempPath;
#else
		MaxSDK::Util::Path projectPath = IPathConfigMgr::GetPathConfigMgr()->GetCurrentProjectFolderPath();
		projectPath.RemoveLeaf();
		path = projectPath.GetCStr();
		path += "\\autoback\\";
#endif
		
		path += "biped2bone.fbx";

	//	MessageBox(NULL, path.c_str(), "Test", MB_OK);
		//
		GetCOREInterface()->ExportToFile(path.c_str());
		
		//
		GetCOREInterface()->FileReset();

		//
		GetCOREInterface()->ImportFromFile(path.c_str());

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

#endif // __Biped2BoneActionItem_h__
