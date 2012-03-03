/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-13ÈÕ   17:10
*********************************************************************/
#ifndef __BuffFIFO_h__
#define __BuffFIFO_h__
#include "Common.h"
namespace Buddha
{
	class CBufFIFO
	{
	public:
		CBufFIFO ();
		~CBufFIFO ();

		/// Push 'buffer' in the head of the FIFO
		void	 push (const std::vector<u8> &buffer) { push (&buffer[0], (u32)buffer.size()); }

		void	 push (const NLMISC::CMemStream &buffer) { push (buffer.buffer(), buffer.length()); }

		void	 push (const u8 *buffer, u32 size);

		/// Concate and push 'buffer1' and buffer2 in the head of the FIFO. The goal is to avoid a copy
		void	 push (const std::vector<u8> &buffer1, const std::vector<u8> &buffer2);

		/// Get the buffer in the tail of the FIFO and put it in 'buffer'
		void	 front (std::vector<u8> &buffer);

		void	 front (NLMISC::CMemStream &buffer);

		void	 front (u8 *&buffer, u32 &size);

		/// This function returns the last byte of the front message
		/// It is used by the network to know a value quickly without doing front()
		u8	 frontLast ();


		/// Pop the buffer in the tail of the FIFO
		void	 pop ();

		/// Set the size of the FIFO buffer in byte
		void	 resize (u32 size);

		/// Return true if the FIFO is empty
		bool	 empty () { return _Empty; }


		/// Erase the FIFO
		void	 clear ();

		/// Returns the size of the FIFO
		u32	 size ();

		/// display the FIFO to stdout (used to debug the FIFO)
		void	 display ();

		/// display the FIFO statistics (speed, nbcall, etc...) to stdout
		void	 displayStats (CLog *log = InfoLog);

	private:

		typedef u32 TFifoSize;

		// pointer to the big buffer
		u8	*_Buffer;
		// size of the big buffer
		u32	 _BufferSize;

		// true if the bufffer is empty
		bool	 _Empty;

		// head of the FIFO
		u8	*_Head;
		// tail of the FIFO
		u8	*_Tail;
		// pointer to the rewinder of the FIFO
		u8	*_Rewinder;

		// return true if we can put size bytes on the buffer
		// return false if we have to resize
		bool	 canFit (u32 size);


		// statistics of the FIFO
		u32 _BiggestBlock;
		u32 _SmallestBlock;
		u32 _BiggestBuffer;
		u32 _SmallestBuffer;
		u32 _Pushed ;
		u32 _Fronted;
		u32 _Resized;
		s64 _PushedTime;
		s64 _FrontedTime;
		s64 _ResizedTime;
	};
}

#endif //__BuffFIFO_h__
