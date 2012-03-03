#include "ListenSock.h"
namespace Shannon
{
	ListenSock::ListenSock():_Bound( false )
	{
	}

	void ListenSock::init( u16 port )
	{
		// Use any address
		Address localaddr; // By default, INETADDR_ANY (useful for gateways that have several ip addresses)
		localaddr.setPort( port );
		init( localaddr );
	}

	void ListenSock::init( const Address& addr )
	{
		// Create socket
		createSocket(SOCK_STREAM, IPPROTO_TCP);
		setBacklog(-1);

		// Bind socket to port
		if ( ::bind( _socket, (const sockaddr *)addr.sockAddr(), sizeof(sockaddr_in) ) != 0 )
		{
			throw ESocket( "Unable to bind listen socket to port" );
		}
		_LocalAddr = addr;
		_Bound = true;

		// Listen
		if ( ::listen( _socket, _BackLog ) != 0 ) // SOMAXCONN = maximum length of the queue of pending connections
		{
			throw ESocket( "Unable to listen on specified port" );
		}
	}

	void ListenSock::setBacklog( s32 backlog )
	{
		if ( backlog == -1 )
		{
			_BackLog = SOMAXCONN; // SOMAXCONN = maximum length of the queue of pending connections
		}
		else
		{
			_BackLog = backlog;
		}
		if ( _Bound )
		{
			if ( ::listen( _socket, _BackLog ) != 0 )
			{
				throw ESocket( "Unable to listen on specified port, while changing backlog" );
			}
		}
	}

	TCPSocket		* ListenSock::accept()
	{
		// Accept connection
		sockaddr_in saddr;
		socklen_t saddrlen = (socklen_t)sizeof(saddr);
		SOCKET newsock = (SOCKET)::accept( _socket, (sockaddr*)&saddr, &saddrlen );
		if ( newsock == INVALID_SOCKET )
		{
			if (_socket == INVALID_SOCKET)
				// normal case, the listen sock have been closed, just return NULL.
				return NULL;
			throw ESocket( "Accept returned an invalid socket");
		}
		// Construct and save a CTcpSock object
		Address addr;
		addr.setSockAddr( &saddr );
		TCPSocket *connection = new TCPSocket( newsock, addr );
		return connection;
	}
}