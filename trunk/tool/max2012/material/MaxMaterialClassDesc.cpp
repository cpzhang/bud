#include "MaxMaterialClassDesc.h"

ClassDesc2* GetMaxMaterialDesc() 
{ 
	static MaxMaterialClassDesc MaxMaterialDesc;
	return &MaxMaterialDesc; 
}
