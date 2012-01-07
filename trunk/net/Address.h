/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:59
*********************************************************************/
#ifndef __Address_h__
#define __Address_h__
#include "Common.h"
namespace Shannon
{
	class Address
	{
	public:
		Address();
		~Address();

		/// Returns if object (address and port) is valid
		bool				isValid() const;

		/// Returns internal socket address (read only)
		const sockaddr_in	 *sockAddr() const;
	private:
		bool				_valid;
		sockaddr_in			*_socketAddress;
	};
}

#endif //__Address_h__
