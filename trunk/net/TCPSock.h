/********************************************************************
	author :	cpzhang
	created:	2011-29-12   17:38
*********************************************************************/
#ifndef __TCPSock_h__
#define __TCPSock_h__
#include "Common.h"
#include "Sock.h"
namespace Shannon
{
	class _NET_EXPORT_ TCPSocket : public Sock
	{
	public:
		TCPSocket();
		/// Construct a CTcpSock object using an already connected socket descriptor and its associated remote address
		TCPSocket( SOCKET sock, const Address& remoteaddr );

		~TCPSocket();
		/** Sets/unsets SO_KEEPALIVE (true by default).
		 */
		void		setKeepAlive( bool keepAlive);

	public:
		virtual void		connect( const Address& addr );
	};
}

#endif //__TCPSock_h__
