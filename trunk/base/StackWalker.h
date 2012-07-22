/********************************************************************
	author :	cpzhang
	created:	2012-1-17   15:20
*********************************************************************/
#ifndef __StackWalker_h__
#define __StackWalker_h__
#include "Common.h"
namespace Zen
{
	class _BUHHDA_EXPORT_ StackWalker
	{
	public:
		class IDumpCallBack
		{
		public:
			virtual void onDump() = 0;
		};
	public:
		StackWalker();
		~StackWalker();

		void init();
		void finalize();
		void DumpMiniDump(PEXCEPTION_POINTERS pExceptionInfo);
		//void DumpMiniDump(PEXCEPTION_POINTERS pExceptionInfo,HANDLE hFile);
		void setDumpCallBack(IDumpCallBack* cb);
	public:
		static StackWalker *GetInstancePtr()
		{
			static StackWalker cd;
			return &cd;
		}
		static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
	private:
		LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
		tstring m_szDumpFileName;
		IDumpCallBack*	_dumpCallBack;
	};
}

#endif //__StackWalker_h__
