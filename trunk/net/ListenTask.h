/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-10ÈÕ   12:26
*********************************************************************/
#ifndef __ListenTask_h__
#define __ListenTask_h__
#include "Common.h"
#include "base/Thread.h"
#include "ListenSock.h"
namespace Shannon
{
	class _NET_EXPORT_ ListenTask: public Zen::IRunnable
	{
	public:
		virtual void run();
	public:
		ListenTask();
		/// Begins to listen on the specified port (call before running thread)
		void			init( u16 port, s32 maxExpectedBlockSize );
		/// Close listening socket
		void			close();
		/// Returns true if the requireExit() has been called
		bool	exitRequired() const;
		/// Tells the task to exit
		void	requireExit();

	private:
		ListenSock			_ListenSock;
		u32					_MaxExpectedBlockSize;
		volatile bool		_ExitRequired;
	};
}

#endif //__ListenTask_h__
