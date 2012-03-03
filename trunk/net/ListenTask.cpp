#include "ListenTask.h"
#include "TCPSock.h"
namespace Shannon
{
	void ListenTask::init( u16 port, s32 maxExpectedBlockSize )
	{
		_ListenSock.init( port );
		_MaxExpectedBlockSize = maxExpectedBlockSize;
	}

	void ListenTask::close()
	{
		_ListenSock.close();
	}

	void ListenTask::run()
	{
		fd_set readers;
		// Accept connections
		while ( ! exitRequired() )
		{
			try
			{
				// Get and setup the new socket
				FD_ZERO( &readers );
				FD_SET( _ListenSock.descriptor(), &readers );
				int res = ::select( 1, &readers, NULL, NULL, NULL ); /// Wait indefinitely

				if ( res == -1)
				{
					continue;
				}
				MessageBox(NULL, "hint", "before accept", MB_OK);
				TCPSocket *newSock = _ListenSock.accept();
				MessageBox(NULL, "hint", "after accept", MB_OK);
				if (newSock)
				{
					u8 buffer[512];
					size_t len = 512;
					//newSock->receive(buffer, len);
					//MessageBox(NULL, (char*)buffer, "server rec", MB_OK);
					std::string sendMessage("hello i am the server");
					len = sendMessage.size();
					newSock->send((u8*)sendMessage.c_str(), len);
					MessageBox(NULL, "hint", "after send", MB_OK);
					sendMessage = "hello i am the server 2";
					newSock->send((u8*)sendMessage.c_str(), len);
					MessageBox(NULL, "hint", "after send 2", MB_OK);
					//newSock->close();
					//delete newSock;
				}
/*
				if (newSock != NULL)
				{
					CServerBufSock *bufsock = new CServerBufSock( newSock );
					LNETL1_DEBUG( "LNETL1: New connection : %s", bufsock->asString().c_str() );
					bufsock->setNonBlocking();
					bufsock->setMaxExpectedBlockSize( _MaxExpectedBlockSize );
					if ( _Server->noDelay() )
					{
						bufsock->Sock->setNoDelay( true );
					}

					// Notify the new connection
					bufsock->advertiseConnection( _Server );

					// Dispatch the socket into the thread pool
					_Server->dispatchNewSocket( bufsock );
				}

*/
				//NbLoop++;
			}
			catch (const ESocket &e)
			{
				// It can occur when too many sockets are open (e.g. 885 connections)
			}
		}
	}

	ListenTask::ListenTask():
		_ExitRequired(false)
	{

	}

	bool ListenTask::exitRequired() const
	{ 
		return _ExitRequired; 
	}

	void ListenTask::requireExit()
	{
		_ExitRequired = true;
	}

}