#include "Thread.h"
#include "Exception.h"
namespace Buddha
{
	DWORD TLSThreadPointer = 0xFFFFFFFF;
	static unsigned long __stdcall ProxyFunc (void *arg)
	{
		Thread *parent = (Thread *) arg;

		// TLS alloc must have been done
		assert (TLSThreadPointer != 0xffffffff);

		// Set the thread pointer in TLS memory
		assert (TlsSetValue (TLSThreadPointer, (void*)parent) != 0);

		// Run the thread
		parent->getRunnable()->run();

		return 0;
	}
	Thread MainThread ((void*)GetCurrentThread (), GetCurrentThreadId());
	Thread::Thread( IRunnable *runnable, u32 stackSize )
	{
		_StackSize = stackSize;
		_Runnable = runnable;
		_ThreadHandle = NULL;
		_SuspendCount = -1;
		_MainThread = false;
	}

	Thread::Thread( void* threadHandle, u32 threadId )
	{
		// Main thread
		_MainThread = true;
		_Runnable = NULL;
		_ThreadHandle = threadHandle;
		_ThreadId = threadId;

		// TLS alloc must have been done
		TLSThreadPointer = TlsAlloc ();

		// Set the thread pointer in TLS memory
		assert(TlsSetValue (TLSThreadPointer, (void*)this) != 0);

		if (GetCurrentThreadId() == threadId)
		{
			_SuspendCount = 0; // is calling thread call this itself, well, if we reach this place
			// there are chances that it is not suspended ...
		}
		else
		{
			CRITICAL_SECTION cs;
			InitializeCriticalSection(&cs);
			EnterCriticalSection(&cs);
			// the 2 following statement must be executed atomicaly among the threads of the current process !
			SuspendThread(threadHandle);
			_SuspendCount = ResumeThread(threadHandle);
			LeaveCriticalSection(&cs);
			DeleteCriticalSection(&cs);
		}
	}

	Thread::~Thread()
	{
		// If not the main thread
		if (_MainThread)
		{
			// Free TLS memory
			TlsFree (TLSThreadPointer);
		}
		else
		{
			if (_ThreadHandle != NULL) terminate();
		}
	}

	Thread* Thread::create( IRunnable *runnable, u32 stackSize /*= 0*/ )
	{
		return new Thread(runnable, stackSize);
	}

	void Thread::terminate()
	{
		TerminateThread((HANDLE)_ThreadHandle, 0);
		CloseHandle((HANDLE)_ThreadHandle);
		_ThreadHandle = NULL;
		_SuspendCount = -1;
	}

	void Thread::start()
	{
		_ThreadHandle = (void *) ::CreateThread (NULL, 0, ProxyFunc, this, 0, (DWORD *)&_ThreadId);
		SetThreadPriorityBoost (_ThreadHandle, TRUE); // FALSE == Enable Priority Boost
		if (_ThreadHandle == NULL)
		{
			throw EThread ( "Cannot create new thread" );
		}

		_SuspendCount = 0;
	}
}