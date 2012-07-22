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
	class ESocket : public Zen::Exception
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
	/// Exception raised when a connection is gracefully closed by peer
	struct ESocketConnectionClosed : public ESocket
	{
		ESocketConnectionClosed() : ESocket( "Connection closed" ) {}
	};
	class _NET_EXPORT_ Sock
	{
	public:
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

		bool				connected() { return _connected; }
		/** Closes the socket (without shutdown)
			* In general you don't need to call this method. But you can call it to:
			* - close a listening socket (i.e. stop accepting connections), or
			* - stop a select() in progress in another thread (in this case, just calling the destructor is not enough)
			*/
		virtual void		close();
		
		/** Sets the socket in nonblocking mode. Call this method *after* connect(), otherwise you will get
		 * an "would block" error (10035 on Windows). In nonblocking mode, use received() and sent() instead of receive() and send()
		 */
		void				setNonBlockingMode ( bool bm );

		enum eSockResult
		{
			eSockResult_Ok,
			eSockResult_WouldBlock,
			eSockResult_ConnectionClosed, 
			eSockResult_Error
		};
	/// @name Sending data
	//@{

	/** Sends a message.
	 *
	 * In blocking mode: the method waits until 'len' bytes have been sent.
	 *
	 * In nonblocking mode : the method resets len to the actual number of bytes sent.
	 * Even if less bytes than expected have been sent, it returns CSock::Ok. The caller
	 * is expected to test the actual len to check if the remaining data must be resent.
	 *
	 * \return CSock::Ok or CSock::Error (in case of failure).
	 * When throw_exception is true, the method throws an ESocket exception in case of failure.
     */
		eSockResult	send( const u8 *buffer, u32& len, bool throw_exception=true );
	
	/** Receive a partial or an entire block of data, depending on nonblocking mode.
	 *
	 * In blocking mode: the method waits until 'len' bytes have been received.
	 *
	 * In nonblocking mode: the method reads the bytes that have already been received only, and
	 * resets 'len' to the number of bytes read. The actual length may be smaller than the demanded
	 * length. In no data is available, the return value is CSock::WouldBlock. If dataAvailable()
	 * returns true, you are sure that receive() will not return CSock::WouldBlock.
	 *
	 * In case of graceful disconnection:
	 * - connected() become false
	 * - the return value is CSock::ConnectionClosed or an ESocketConnectionClosed exception is thrown.
	 *
	 * In case of failure (e.g. connection reset by peer) :
	 * - the return value is CSock::Error or an ESocket exception is thrown.
	 * You may want to close the connection manually.
	 */
	eSockResult	receive( u8 *buffer, u32& len, bool throw_exception=true );

	/// Returns the socket descriptor
	SOCKET				descriptor() const { return _socket; }
	protected:
		Sock();
		/// Construct a CSock object using an existing connected socket descriptor and its associated remote address
		Sock( SOCKET sock, const Address& remoteaddr );

		/// Creates the socket and get a valid descriptor
		void			createSocket( int type, int protocol );


		/// Sets the local address
		void			setLocalAddress();

	protected:
		SOCKET			_socket;

		/// True after calling connect()
		volatile bool	_connected;
		/// If true, the socket is in nonblocking mode
		bool			_NonBlocking;
		/// Number of bytes sent on this socket
		u64			_BytesSent;
		/// Number of bytes received on this socket
		u64			_BytesReceived;
		/// Address of local host (valid if connected)
		Address		_LocalAddr;
		/// Address of the remote host (valid if connected)
		Address	_RemoteAddr;
	private:
		/// True if the network library has been initialized
		static bool		_Initialized;
	};
}

#endif //__Sock_h__