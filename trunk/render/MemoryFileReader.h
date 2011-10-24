//==========================================================================
/**
* @file	  : MemoryFileReader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-20   10:54
* purpose : 
*/
//==========================================================================

#ifndef __MemoryFileReader_h__
#define __MemoryFileReader_h__

#include "Common.h"
#include "IFileRead.h"

namespace Euclid
{
	class _EuclidExport_  MemoryFileReader : public IFileRead
	{
	public:
		virtual unsigned int read(void* buff, unsigned int size);
		virtual bool seek(long step, bool relative2CurrentCursor = true);
		virtual bool seek2Begin();
		virtual long getCursor() const;
		virtual long getSize() const;
		virtual u32 getUnreadSize() const;
		virtual bool isEnd() const;
	public:
		MemoryFileReader(void* memory, long len);
		~MemoryFileReader();

	private:
		void*			_buffer;
		unsigned int	_cursor;
		unsigned int	_length;
	};
}

#endif // __MemoryFileReader_h__
 
