// Alpha.cpp : main source file for Alpha.exe
//
#include <atlbase.h>
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlmisc.h>
#include "mainFrame.h"

// global variable
CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// Ensures that the common control DLL (Comctl32.dll) is loaded, and registers specific common control classes from the DLL.
	// An application must call this function before creating a common control.
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	//Closes the COM library on the current thread, unloads all DLLs loaded by the thread, 
	//frees any other resources that the thread maintains, and forces all RPC connections on the thread to close.
	::CoUninitialize();

	return nRet;
}
