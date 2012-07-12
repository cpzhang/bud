/********************************************************************
**	file: 		MaxGUPClassDesc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-27
**	purpose:	
*********************************************************************/
#ifndef __MaxGUPClassDesc_h__
#define __MaxGUPClassDesc_h__

#include "MaxGUPCommon.h"
#include "ParticlePreviewActionItem.h"
#include "Biped2BoneActionItem.h"
#include "OnlyPreviewSelectedActionItem.h"

class MaxGUPClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new MaxGUP(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxGUP_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return "Global Utility PlugIn"; }

	virtual const TCHAR* InternalName() 			{ return _T("MaxGUP"); }
	virtual HINSTANCE HInstance() 					{ return hInstance; }

	//You only need to add the action stuff to one Class Desc
	int             NumActionTables() { return 1; }
	ActionTable*  GetActionTable(int i) { return GetActions(); }

	ActionTable* GetActions()
	{
		TSTR name = ("Unnamed Action Table");
		ActionTable* pTab;
		pTab = new ActionTable(kUnwrapActions, kUnwrapContext, name);        
		
		{
			ParticlePreviewActionItem* ta = new ParticlePreviewActionItem;
			ta->_id = eActionItem_PreviewAll;
			pTab->AppendOperation(ta);
		}


		{
			OnlyPreviewSelectedActionItem* ta = new OnlyPreviewSelectedActionItem;
			ta->_id = eActionItem_PreviewSelected;
			pTab->AppendOperation(ta);
		}

		{
			Biped2BoneActionItem* ta = new Biped2BoneActionItem;
			ta->_id = eActionItem_Biped2Bone;
			pTab->AppendOperation(ta);
		}


		GetCOREInterface()->GetActionManager()->RegisterActionContext(kUnwrapContext, name.data());
		return pTab;
	}

};

ClassDesc2* GetMaxGUPDesc() 
{ 
	static MaxGUPClassDesc MaxGUPDesc;
	return &MaxGUPDesc; 
}

#endif // __MaxGUPClassDesc_h__
