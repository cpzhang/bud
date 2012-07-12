#include "PlatoCommon.h"
#include "PlatoExporter.h"
#include "PlatoDescriptor.h"

HINSTANCE gInstance;
/*
	This function is the DLL's entry point function called by Windows when the DLL is loaded. 
	It may be called many times during time critical operations like rendering. 
	DllMain() should do nothing except store the DLL's HINSTANCE, and call DisableThreadLibraryCalls() if nothing special needs to be done during DLL_THREAD_ATTACH and DLL_THREAD_DETACH. 
	Note that you should never throw an exception in DllMain().
	Once called for the first time, two main actions are usualy performed:
		1. A handle to the DLL module in the OS memory space is assigned to a global variable in the plug-in. This handle is provided by OS as an argument in its call of DLLMain(), 
		and will be used later by other functions in the plug-in. 
		2. Further notifications from OS get disabled for this DLL. 
		This means that if other threads are created in the same OS process and decide to load the same DLL, the DLLMain() will not be called anymore
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{ 
	if( fdwReason == DLL_PROCESS_ATTACH )  
	{     // Hang on to this DLL's instance handle. 
		gInstance = hinstDLL;   
		DisableThreadLibraryCalls(gInstance); 
	}  
	return(TRUE);
} 

/*
	This function returns a string that describes the DLL. 
	This is used by 3ds Max to identify entities referenced in a scene when the implementing DLL is not available.
	For example if a particular material is used in a scene which is shared with another 3ds Max user who does not have the appropriate DLL on their system. 
	The string should therefore mention where the user could obtain the DLL.
*/
const TCHAR* LibDescription()
{   
	return PlatoExporter::tLongDesc.c_str();
} 

 /*
	This function returns the number of plug-in classes this DLL uses.
 */
int LibNumberClasses()
{  
	return 1;
}

/*
	This function returns a class descriptor (ClassDesc2) for the nth class exported by the DLL.
*/
ClassDesc* LibClassDesc(int i)
{ 
	if (i == 0)
	{
		return GetPlatoDescriptor();
	}

	return NULL;
}

/*
	This function returns a pre-defined constant indicating the version of the system under which it was compiled. 
	It is used to allow 3ds Max identify obsolete DLLs. 
	This function is called once, right after your plug-in has been loaded by 3ds Max.
	Normally you would just return Get3DSMAXVersion().
*/

ULONG LibVersion()
{  
	return Get3DSMAXVersion();
} 

 /*
	Perform one-time plug-in initialization in this method. 
	Return TRUE if your plug-in has initialization was successful (e.g. the plug-in has loaded correctly), or FALSE otherwise. 
	If the function returns FALSE, the system will not load the plug-in, it will then call FreeLibrary() on your DLL, and send you a message.
*/
int LibInitialize()
{  
	// TODO: Perform initialization here. 
	return TRUE;
} 

/*
	 Called once, just before the plug-in is unloaded. 
	 Perform one-time plug-in uninitialization in this method.
	 The system ignores the return value.
*/
int LibShutdown()
{ 
	// TODO: Perform uninitialization here. 
	return TRUE;
}

ULONG CanAutoDefer()
{
	return 0;
}
