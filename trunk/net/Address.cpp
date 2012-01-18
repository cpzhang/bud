#include "Address.h"
#include "Sock.h"
namespace Shannon
{


	Address::Address()
		:_valid(false), _socketAddress(NULL)
	{

	}

	Address::Address( const std::string& hostName, u16 port )
	{
		init();
		setPort(port);
		setByName(hostName);
	}

	Address::~Address()
	{

	}

	bool Address::isValid() const
	{
		return _valid && (_socketAddress->sin_port!= 0);
	}

	const sockaddr_in	 * Address::sockAddr() const
	{
		return _socketAddress;
	}

	void Address::init()
	{
		Sock::InitNet();

		_valid = false;

		_socketAddress = new sockaddr_in;
		_socketAddress->sin_family = AF_INET;
		memset( _socketAddress->sin_zero, 0, 8 );
	}

	void Address::setPort( u16 port )
	{
		_socketAddress->sin_port = htons(port);
	}

	Address& Address::setByName( const std::string& hostname )
	{
		// Try to convert directly for addresses such as a.b.c.d
		in_addr iaddr;
		iaddr.s_addr = inet_addr( hostname.c_str() );
		if ( iaddr.s_addr == INADDR_NONE )
		{
			// Otherwise use the traditional DNS look-up
			struct addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
			hints.ai_socktype = SOCK_STREAM;

			struct addrinfo *res = NULL;
			int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
			if (status)
			{
				_valid = false;
				// return *this;
				throw ESocket( (std::string("Hostname resolution failed for ") + hostname).c_str() );
			}
			struct addrinfo *p = res;

			// process all addresses
			while (p != NULL)
			{
				// check address family
				if (p->ai_family == AF_INET)
				{
					// ipv4
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;

					// convert the IP to a string
					_hostName = std::string(inet_ntoa(ipv4->sin_addr));
					memcpy( &_socketAddress->sin_addr, &ipv4->sin_addr, sizeof(in_addr) );
				}
				else if (p->ai_family == AF_INET6)
				{
					// ipv6
					// TODO: modify class to be able to handle IPv6

					// struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;

					// convert the IP to a string
					// inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
					// memcpy( &_SockAddr->sin_addr, &ipv6->sin_addr, sizeof(in_addr) );
				}

				// process next address
				p = p->ai_next;
			}

			// free the linked list
			freeaddrinfo(res);
		}
		else
		{
			_hostName = hostname;
			memcpy( &_socketAddress->sin_addr, &iaddr, sizeof(iaddr) );
		}
		_valid = true;
		return *this;
	}
}