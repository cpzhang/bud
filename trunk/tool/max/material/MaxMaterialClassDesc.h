/********************************************************************
**	file: 		MaxMaterialClassDesc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __MaxMaterialClassDesc_h__
#define __MaxMaterialClassDesc_h__

#include "MaxCommon.h"
#include "MaxMaterial.h"

class MaxMaterialClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL loading = FALSE) 		{ return new MaxMaterial(loading); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return MATERIAL_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MaxMaterial_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("MaxMaterial"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
};

ClassDesc2* GetMaxMaterialDesc();

#endif // __MaxMaterialClassDesc_h__
