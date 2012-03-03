#include "TCPSock.h"
namespace Shannon
{


	TCPSocket::TCPSocket()
	{

	}

	TCPSocket::TCPSocket( SOCKET sock, const Address& remoteaddr )
		:Sock( sock, remoteaddr )
	{

	}


	TCPSocket::~TCPSocket()
	{

	}

	void TCPSocket::connect( const Address& addr )
	{
		if (_socket != INVALID_SOCKET)
		{
			close();
		}
		createSocket(SOCK_STREAM, IPPROTO_TCP);
		// activate keep alive
		setKeepAlive(true);

		// Connection
		Sock::connect( addr );
	}

	void TCPSocket::setKeepAlive( bool keepAlive )
	{
		int b = keepAlive?1:0;
		if ( setsockopt( _socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&b, sizeof(b) ) != 0 )
		{
			throw ESocket( "setKeepAlive failed" );
		}
	}

}