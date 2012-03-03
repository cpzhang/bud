/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-13ÈÕ   11:17
*********************************************************************/
#ifndef __BufferBase_h__
#define __BufferBase_h__
#include "Common.h"
namespace Shannon
{
	class BufferBase
	{
	public:
		BufferBase();
		~BufferBase();
	public:
		/// Returns the max size of the received messages (default: 2^31-1)
		u32	maxExpectedBlockSize() const
		{
			return _MaxExpectedBlockSize;
		}

		/// Returns the max size of the sent messages (default: 2^31-1)
		u32	maxSentBlockSize() const
		{
			return _MaxSentBlockSize;
		}
		/**
		 * Sets the max size of the received messages.
		 * If receiving a message bigger than the limit, the connection will be dropped.
		 *
		 * Default value: CBufNetBase::DefaultMaxExpectedBlockSize
		 * If you put a negative number as limit, the max size is reseted to the default value.
		 * Warning: you can call this method only at initialization time, before connecting (for a client)
		 * or calling init() (for a server) !
		 */
		void	setMaxExpectedBlockSize( s32 limit )
		{
			if ( limit < 0 )
				_MaxExpectedBlockSize = DefaultMaxExpectedBlockSize;
			else
				_MaxExpectedBlockSize = (u32)limit;
		}

		/**
		 * Sets the max size of the sent messages.
		 * Any bigger sent block will produce an assertion failure, currently.
		 *
		 * Default value: CBufNetBase::DefaultMaxSentBlockSize
		 * If you put a negative number as limit, the max size is reseted to the default value.
		 * Warning: you can call this method only at initialization time, before connecting (for a client)
		 * or calling init() (for a server) !
		 */
		void	setMaxSentBlockSize( s32 limit )
		{
			if ( limit < 0 )
				_MaxSentBlockSize = DefaultMaxSentBlockSize;
			else
				_MaxSentBlockSize = (u32)limit;
		}
	public:
		/// The value that will be used if setMaxExpectedBlockSize() is not called (or called with a negative argument)
		static u32 DefaultMaxExpectedBlockSize;

		/// The value that will be used if setMaxSentBlockSize() is not called (or called with a negative argument)
		static u32 DefaultMaxSentBlockSize;
	private:
		/// The receive queue, protected by a mutex-like device
//		CSynchronizedFIFO	_RecvFifo;

		/// Max size of received messages (limited by the user)
		u32				_MaxExpectedBlockSize;

		/// Max size of sent messages (limited by the user)
		u32				_MaxSentBlockSize;
	};
}

#endif //__BufferBase_h__
