/********************************************************************
**	file: 		MaxDlgProc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __MaxDlgProc_h__
#define __MaxDlgProc_h__

#include "MaxCommon.h"

class MaxMaterialDlgProc : public ParamMap2UserDlgProc 
{
public:
	INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) 
		{
		case WM_INITDIALOG:
			
			return TRUE;
		}
		return FALSE;
	}
	void DeleteThis() { }
};

static MaxMaterialDlgProc tMaxMaterialDlgProc;
#endif // __MaxDlgProc_h__
