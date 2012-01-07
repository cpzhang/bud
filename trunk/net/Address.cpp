#include "Address.h"
namespace Shannon
{


	Address::Address()
		:_valid(false), _socketAddress(NULL)
	{

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
}