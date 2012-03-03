/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-7ÈÕ   11:37
*********************************************************************/
#ifndef __ListenSock_h__
#define __ListenSock_h__
#include "TCPSock.h"
namespace Shannon
{
	class _NET_EXPORT_ ListenSock: public TCPSocket
	{
	public:
		ListenSock();
		/// Prepares to receive connections on a specified port (bind+listen)
		void			init( u16 port );

		/// Prepares to receive connections on a specified address/port (useful when the host has several addresses)
		void			init( const Address& addr );
		/// Sets the number of the pending connections queue, or -1 for the maximum possible value.
		void			setBacklog( s32 backlog );
		/// Blocks until an incoming connection is requested, accepts it, and creates a new socket (you have to delete it after use)
		TCPSocket		*accept();
	private:
		bool			_Bound;
		s32				_BackLog;
	};
}

#endif //__ListenSock_h__
