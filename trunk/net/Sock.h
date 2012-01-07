/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:05
*********************************************************************/
#ifndef __Sock_h__
#define __Sock_h__
#include "Common.h"
#include "Address.h"
namespace Shannon
{
	/**
	* Network exceptions
	* \author Olivier Cado
	* \author Nevrax France
	* \date 2000
	*/
	class ESocket : public Buddha::Exception
	{
	public:
		/** Constructor
			* You can provide an internet address. If so, reason *must* contain "%s"
			* where the address should be written. Moreover, the length of reason plus
			* the length of the address when displayed by asString() should no exceed 256.
			*/
		ESocket( const char *reason="", bool systemerror=true, Address *addr=NULL );
	};

	/// Exception raised when connect() fails
	struct ESocketConnectionFailed : public ESocket
	{
		ESocketConnectionFailed( Address addr ) : ESocket( "Connection to %s failed", true, &addr ) {}
	};

	class _NET_EXPORT_ Sock
	{
	public:
		Sock();
		~Sock();
		/*
		* Initializes the network engine if it is not already done (under Windows, calls WSAStartup()).
		*/
		static void InitNet();
		
		/// Releases the network engine
		static void	DestroyNet();

		/** Returns the code of the last error that has occured.
		 * Note: This code is platform-dependant. On Unix, it is errno; on Windows it is the Winsock error code.
		 * See also errorString()
		 */
		static int GetLastError();

		/// Returns a string explaining the network error (see getLastError())
		static std::string	GetErrorString( int errorcode );

		/** Connection.
		 * This method does not return a boolean, otherwise a programmer could ignore the result and no
		 * exception would be thrown if connection fails :
		 * - If addr is not valid, an exception ESocket is thrown
		 * - If connect() fails for another reason, an exception ESocketConnectionFailed is thrown
		 */
		virtual void		connect( const Address& addr );

		
		/** Closes the socket (without shutdown)
			* In general you don't need to call this method. But you can call it to:
			* - close a listening socket (i.e. stop accepting connections), or
			* - stop a select() in progress in another thread (in this case, just calling the destructor is not enough)
			*/
		virtual void		close();
	protected:
		/// Creates the socket and get a valid descriptor
		void			createSocket( int type, int protocol );


		/// Sets the local address
		void			setLocalAddress();

	protected:
		SOCKET			_socket;

		/// True after calling connect()
		volatile bool	_connected;
	private:
		/// True if the network library has been initialized
		static bool		_Initialized;
	};
}

#endif //__Sock_h__