#include "StackWalker.h"
namespace Buddha
{

	StackWalker::StackWalker()
		:m_szDumpFileName(TEXT("crash.dmp")),
		m_previousFilter(NULL),
		_dumpCallBack(NULL)
	{
		
	}

	StackWalker::~StackWalker()
	{

	}

	void StackWalker::init()
	{
		m_previousFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);
	}

	void StackWalker::finalize()
	{
		if (m_previousFilter)
		{
			SetUnhandledExceptionFilter(m_previousFilter);
		}
	}

	LONG WINAPI StackWalker::UnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo )
	{
		if (pExceptionInfo == NULL) 
		{
			// Generate exception to get proper context in dump
			__try 
			{
				RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
			} 
			__except(GetInstancePtr()->DumpMiniDump(GetExceptionInformation()),
				EXCEPTION_CONTINUE_EXECUTION) 
			{
			}
		}
		else
		{
			GetInstancePtr()->DumpMiniDump(pExceptionInfo);
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void StackWalker::DumpMiniDump( PEXCEPTION_POINTERS pExceptionInfo )
	{
		//
		HANDLE hMiniDumpFile = CreateFile(
			m_szDumpFileName.c_str(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
			NULL);
		//
		if (hMiniDumpFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION eInfo;
			eInfo.ThreadId = GetCurrentThreadId();
			eInfo.ExceptionPointers = pExceptionInfo;
			eInfo.ClientPointers = FALSE;

			// note:  MiniDumpWithIndirectlyReferencedMemory does not work on Win98
			::MiniDumpWriteDump(
				::GetCurrentProcess(),
				::GetCurrentProcessId(),
				hMiniDumpFile,
				(MINIDUMP_TYPE)MiniDumpWithIndirectlyReferencedMemory,
				pExceptionInfo ? &eInfo : NULL,
				NULL,
				NULL);
			//
			CloseHandle(hMiniDumpFile);
			if (_dumpCallBack)
			{
				_dumpCallBack->onDump();
			}
		}
	}

	void StackWalker::setDumpCallBack( IDumpCallBack* cb )
	{
		_dumpCallBack = cb;
	}
}