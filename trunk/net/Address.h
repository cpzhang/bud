/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:59
*********************************************************************/
#ifndef __Address_h__
#define __Address_h__
#include "Common.h"
namespace Shannon
{
	class _NET_EXPORT_ Address
	{
	public:
		Address();
		Address( const std::string& hostName, u16 port );
		~Address();

		/// Returns if object (address and port) is valid
		bool				isValid() const;

		/// Returns internal socket address (read only)
		const sockaddr_in	 *sockAddr() const;
		void				setPort( u16 port );
		Address&			setByName( const std::string& hostname );
		/** Sets internal socket address directly (contents is copied).
		* It also retrieves the host name if CInetAddress::RetrieveNames is true.
		*/
		void				setSockAddr( const sockaddr_in* saddr );
	private:
		void				init();
		
	private:
		bool				_valid;
		sockaddr_in			*_socketAddress;
		std::string			_hostName;
	};
}

#endif //__Address_h__
