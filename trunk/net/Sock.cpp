#include "Sock.h"
#include "Address.h"
namespace Shannon
{
	void Sock::InitNet()
	{
		if (!Sock::_Initialized)
		{
			WORD winsock_version = MAKEWORD( 2, 0 );
			WSADATA wsaData;
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
		:_socket(INVALID_SOCKET), _connected(false)
	{

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

	ESocket::ESocket(const char *reason/*=""*/, bool systemerror/*=true*/, Address *addr/*=NULL */ )
	{
		std::ostringstream ss;
		ss<<"Socket error: "<<Sock::GetErrorString(Sock::GetLastError());
		_reason = ss.str();
	}
	bool Sock::_Initialized(false);
}