#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "IPathConfigMgr.h"
//SIMPLE TYPE
#include "imenuman.h"
#include <string>
#include <vector>
#include <guplib.h>


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define MaxGUP_CLASS_ID	Class_ID(0xc6daa199, 0x81400a32)

const ActionTableId kUnwrapActions = 0x7bd55f44;
const ActionContextId kUnwrapContext = 0x7bdf5e33;
const int   kUnwrapMenuBar = 2202134;

enum eActionItem
{
	eActionItem_PreviewAll,
	eActionItem_PreviewSelected,
	eActionItem_Biped2Bone,
	eActionItem_Size,
};