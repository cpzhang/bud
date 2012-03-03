#include "Sock.h"
#include "Address.h"
namespace Shannon
{
	void Sock::InitNet()
	{
		if (!Sock::_Initialized)
		{
			WORD winsock_version = MAKEWORD( 2, 2 );
			WSADATA wsaData;
			// The WSAStartup function initiates use of the Winsock DLL by a process.
			if ( WSAStartup( winsock_version, &wsaData ) != 0 )
			{
				throw ESocket( "Winsock initialization failed" );
			}
			Sock::_Initialized = true;
		}
	}

	int Sock::GetLastError()
	{
		return WSAGetLastError();
	}

	std::string Sock::GetErrorString( int errorcode )
	{
		switch( errorcode )
		{
		case WSAEINTR		 /*10004*/: return "Blocking operation interrupted";
		case WSAEINVAL		 /*10022*/: return "Invalid socket (maybe not bound) or argument";
		case WSAEMFILE		 /*10024*/: return "Too many open sockets";
		case WSAENOTSOCK	 /*10038*/: return "Socket operation on nonsocket (maybe invalid select descriptor)";
		case WSAEMSGSIZE	 /*10040*/: return "Message too long";
		case WSAEADDRINUSE   /*10048*/: return "Address already in use (is this service already running in this computer?)";
		case WSAEADDRNOTAVAIL/*10049*/: return "Address not available";
		case WSAENETDOWN	 /*10050*/: return "Network is down";
		case WSAENETUNREACH  /*10051*/: return "Network is unreachable";
		case WSAECONNRESET   /*10054*/: return "Connection reset by peer";
		case WSAENOBUFS		 /*10055*/: return "No buffer space available; please close applications or reboot";
		case WSAESHUTDOWN	 /*10058*/: return "Cannot send/receive after socket shutdown";
		case WSAETIMEDOUT	 /*10060*/: return "Connection timed-out";
		case WSAECONNREFUSED /*10061*/:	return "Connection refused, the server may be offline";
		case WSAEHOSTUNREACH /*10065*/: return "Remote host is unreachable";
		case WSANOTINITIALISED /*093*/: return "'Windows Sockets' not initialized";
		default:						return "Unkown";
		}
	}

	void Sock::DestroyNet()
	{
		WSACleanup();
		Sock::_Initialized = false;
	}

	void Sock::connect( const Address& addr )
	{
		// Check address
		if (!addr.isValid() )
		{
			throw ESocket( "Unable to connect to invalid address", false );
		}
		// Connection (when _Sock is a datagram socket, connect establishes a default destination address)
		if ( ::connect( _socket, (const sockaddr *)(addr.sockAddr()), sizeof(sockaddr_in) ) != 0 )
		{
			throw ESocketConnectionFailed( addr );
		}
		setLocalAddress();
		_connected = true;
		_BytesReceived = 0;
		_BytesSent = 0;
	}

	void Sock::createSocket( int type, int protocol )
	{
		_socket = (SOCKET)socket( AF_INET, type, protocol );
		if ( _socket == INVALID_SOCKET )
		{
			throw ESocket( "Socket creation failed" );
		}
	}

	Sock::Sock()
		:_socket(INVALID_SOCKET), _connected(false), _NonBlocking(false), _BytesReceived(0), _BytesSent(0)
	{

	}

	Sock::Sock( SOCKET sock, const Address& remoteaddr ):
		_socket( sock ),
		_RemoteAddr( remoteaddr ),
		_NonBlocking( false ),
		_BytesReceived( 0 ),
		_BytesSent( 0 )
	{
		_connected = true;
		// Check remote address
		if ( ! _RemoteAddr.isValid() )
		{
			throw ESocket( "Could not init a socket object with an invalid address", false );
		}
		// Get local socket name
		setLocalAddress();
	}

	Sock::~Sock()
	{

	}

	void Sock::close()
	{
		SOCKET sockToClose = _socket;
		// preset to invalid to bypass exception in listen thread
		_socket = INVALID_SOCKET;
		closesocket(sockToClose);
		_connected = false;
	}

	void Sock::setLocalAddress()
	{

	}

	Sock::eSockResult Sock::send( const u8 *buffer, u32& len, bool throw_exception/*=true */ )
	{
		/*
		If no error occurs, send returns the total number of bytes sent, 
		which can be less than the number requested to be sent in the len parameter.
		Otherwise, a value of SOCKET_ERROR is returned,
		and a specific error code can be retrieved by calling WSAGetLastError.
		*/
		len = ::send(_socket, (const char*)buffer, len, 0);
		if ((int)len == SOCKET_ERROR)
		{
			return eSockResult_Error;
		}
		return eSockResult_Ok;
	}

	Sock::eSockResult Sock::receive( u8 *buffer, u32& len, bool throw_exception/*=true */ )
	{
		if (_NonBlocking)
		{
			len = ::recv( _socket, (char*)buffer, len, 0);
			switch(len)
			{
				// Graceful disconnection
			case 0:
				{
					_connected = false;
					if ( throw_exception )
					{
						throw ESocketConnectionClosed();
					}
					return Sock::eSockResult_ConnectionClosed;
				}break;
				// Socket error or call would block
			case SOCKET_ERROR:
				{
					len = 0;
					if ( GetLastError() == WSAEWOULDBLOCK)
					{
						// Call would block
						return Sock::eSockResult_WouldBlock;
					}
					else
					{
						// Socket error
						if ( throw_exception )
						{
							throw ESocket( "Unable to receive data" );
						}
						return Sock::eSockResult_Error;
					}
				}
			}
		} 
		else // Blocking Mode
		{
			// Receive incoming message, waiting until a complete message has arrived
			u32 total = 0;
			u32 brecvd;
			while ( total < len )
			{
				brecvd = ::recv( _socket, (char*)(buffer+total), len-total, 0 );
				switch ( brecvd )
				{
					// Graceful disconnection
					case 0:
					{
						_connected = false;
						len = total;
						_BytesReceived += len;

						if ( throw_exception )
						{
							throw ESocketConnectionClosed();
						}
						return eSockResult_ConnectionClosed;
					}

					// Socket error
					case SOCKET_ERROR:
					{
						len = total;
						_BytesReceived += len;

						if ( throw_exception )
						{
							throw ESocket( "Unable to receive data" );
						}
						return eSockResult_Error;
					}
				}
				if (brecvd < len - total)
				{
					break;
				}
				total += brecvd;
			}
		}

		_BytesReceived += len;
		return eSockResult_Ok;
	}

	void Sock::setNonBlockingMode( bool bm )
	{
		if ( _NonBlocking != bm )
		{
			u_long b = bm;
			if ( ioctlsocket( _socket, FIONBIO, &b ) != 0 )
			{
				throw ESocket( "Cannot set nonblocking mode" );
			}
			_NonBlocking = bm;
		}
	}

	ESocket::ESocket(const char *reason/*=""*/, bool systemerror/*=true*/, Address *addr/*=NULL */ )
	{
		std::ostringstream ss;
		ss<<"Socket error: "<<Sock::GetErrorString(Sock::GetLastError());
		_reason = ss.str();
	}
	bool Sock::_Initialized(false);
}