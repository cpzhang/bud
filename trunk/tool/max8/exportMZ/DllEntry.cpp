#include "PlatoCommon.h"
#include "PlatoExporter.h"
#include "PlatoDescriptor.h"

HINSTANCE gInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{ 
	if( fdwReason == DLL_PROCESS_ATTACH )  
	{     // Hang on to this DLL's instance handle. 
		gInstance = hinstDLL;   
		DisableThreadLibraryCalls(gInstance); 
	}  
	return(TRUE);
} 

// a string that describes the DLL
const TCHAR* LibDescription()
{   
	return PlatoExporter::tLongDesc.c_str();
} 

// the number of plug-in classes this DLL uses. 
int LibNumberClasses()
{  
	return 1;
}

//  class descriptor 
ClassDesc* LibClassDesc(int i)
{ 
	if (i == 0)
	{
		return GetPlatoDescriptor();
	}

	return NULL;
}

// This function returns a pre-defined constant indicating the version of the system under which it was compiled.
// It is used to allow 3ds Max identify obsolete DLLs. This function is called once, right after your plug-in has been loaded by 3ds Max.
// Normally you would just return Get3DSMAXVersion(). 

ULONG LibVersion()
{  
	return Get3DSMAXVersion();
} 

// Perform one-time plug-in initialization in this method
int LibInitialize()
{  
	// TODO: Perform initialization here. 
	return TRUE;
} 

// Called once, just before the plug-in is unloaded
int LibShutdown()
{ 
	// TODO: Perform uninitialization here. 
	return TRUE;
}

ULONG CanAutoDefer()
{
	return 0;
}
