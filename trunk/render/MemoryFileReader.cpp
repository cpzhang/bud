#include "MemoryFileReader.h"

namespace Euclid
{
	MemoryFileReader::MemoryFileReader(void* memory, long len)
		:	_buffer(memory), _cursor(0), _length(len)
	{

	}

	MemoryFileReader::~MemoryFileReader()
	{
		//safeDeleteArray(_buffer);
		_buffer = NULL;
		_length = 0;
	}

	unsigned int MemoryFileReader::read( void* buff, unsigned int size )
	{
		if (_cursor + size > _length)
		{
			return 0;
		}

		memcpy(buff, (char*)_buffer + _cursor, size);

		_cursor += size;

		return size;
	}

	bool MemoryFileReader::seek( long step, bool relative2CurrentCursor /*= true*/ )
	{
		if (relative2CurrentCursor)
		{
			if (step + _cursor > _length)
			{
				return false;
			}

			_cursor += step;
		}
		else
		{
			if (step > _length)
			{
				return false;
			}

			_cursor = step;
		}

		return true;
	}

	long MemoryFileReader::getCursor() const
	{
		return _cursor;
	}

	long MemoryFileReader::getSize() const
	{
		return _length;
	}

	bool MemoryFileReader::isEnd() const
	{
		return _cursor >= _length;
	}

	u32 MemoryFileReader::getUnreadSize() const
	{
		return _length - _cursor;
	}

	bool MemoryFileReader::seek2Begin()
	{
		_cursor = 0;

		return true;
	}

}
