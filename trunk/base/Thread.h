/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-10ÈÕ   11:32
*********************************************************************/
#ifndef __Thread_h__
#define __Thread_h__
#include "Common.h"
namespace Zen
{
	class IRunnable
	{
	public:
		// Called when a thread is run.
		virtual void run() = 0;
		virtual ~IRunnable()
		{
		}
		// Return the runnable name
		virtual void getName (std::string &result) const
		{
			result = "NoName";
		}
	};
	class _BUHHDA_EXPORT_ Thread
	{
	public:
		/// Constructor
		Thread(IRunnable *runnable, u32 stackSize);
		/// Don't use this constructor, only used to initialise the main thread class
		Thread (void* threadHandle, u32 threadId);

		~Thread();
		/**
		  * Create a new thread.
		  */
		static Thread* create(IRunnable *runnable, u32 stackSize = 0);
		void start();
		bool isRunning();
		void terminate();
		void wait();
		bool setCPUMask(u64 cpuMask);
		u64 getCPUMask();
		std::string getUserName();

		IRunnable *getRunnable()
		{
			return _Runnable;
		}
	private:
		IRunnable*	_Runnable;
		int			_SuspendCount;
		u32			_StackSize;
		void*		_ThreadHandle;	// HANDLE	don't put it to avoid including windows.h
		u32			_ThreadId;		// DWORD	don't put it to avoid including windows.h
		bool		_MainThread;	// true if the thread is the main thread, else false
	};
}

#endif //__Thread_h__
