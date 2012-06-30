#include "DataChunk.h"
#include "StlHelper.h"

namespace Buddha
{
	MemoryFileWriter::MemoryFileWriter()
	{
		_cursor = 0;
		_length = 0;
		_buffer = NULL;
	}
	MemoryFileWriter::~MemoryFileWriter()
	{
		if (_buffer)
		{
			delete _buffer;
			_buffer = NULL;
		}

		_cursor = 0;
		_length = 0;
	}
	bool MemoryFileWriter::isEnd()
	{
		return _cursor >= _length;
	}

	bool MemoryFileWriter::isOverFlow(size_t s)
	{
		return s + _cursor >= _length;
	}

	bool MemoryFileWriter::reallocate(size_t s)
	{
		if (s == 0)
		{
			return false;
		}

		size_t newSize = _cursor + s;

		void* b = new char[newSize];
		if (b == NULL)
		{
			return false;
		}
		memset(b, 0, newSize);
		if (_cursor > 0)
		{
			memcpy(b, _buffer, _cursor);
		}

		safeDelete(_buffer);
		_buffer = b;

		_length = newSize;

		return true;
	}

	bool MemoryFileWriter::write(void* b, size_t s)
	{
		if (isOverFlow(s))
		{
			if (!reallocate(s * 2))
			{
				return false;
			}

		}
		_writeImp(b, s);

		return true;
	}

	bool MemoryFileWriter::_writeImp(void* b, size_t s)
	{
		if (b == NULL)
		{
			return false;
		}
		memcpy((char*)_buffer + _cursor, (char*)b, s);
		_cursor += s;
		return true;
	}

	size_t MemoryFileWriter::getSize()
	{
		if (_cursor == 0)
		{
			return 0;
		}

		return _cursor;
	}

	void* MemoryFileWriter::getBuffer()
	{
		return _buffer;
	}


	ChunkSet::ChunkSet()
	{
		_currentChunk = NULL;
	}
	ChunkSet::~ChunkSet()
	{
		_currentChunk = NULL;
		STLDeleteSequence(_chunkSet);
	}
	bool ChunkSet::beginChunk(const std::string& name)
	{
		if (name.size() != 4)
		{
			return false;
		}

		sChunk* c = new sChunk;
		c->name = name;
		_chunkSet.push_back(c);
		_currentChunk = c;

		return true;
	}

	void ChunkSet::endChunk()
	{
		_currentChunk = NULL;
	}

	bool ChunkSet::write(const char* b, size_t s)
	{
		return write((void*)b, s);
	}

	bool ChunkSet::write(void* b, size_t s)
	{
		if (_currentChunk == NULL)
		{
			return false;
		}

		return _currentChunk->buffer.write(b, s);
	}

	void ChunkSet::save(const std::string& fileName)
	{
		std::ofstream o(fileName.c_str(), std::ios::binary);

		for (size_t i = 0; i != _chunkSet.size(); ++i)
		{
			size_t s = _chunkSet[i]->buffer.getSize();

			std::string name = _chunkSet[i]->name;
			char c[4];
			c[0] = name[3];
			c[1] = name[2];
			c[2] = name[1];
			c[3] = name[0];

			o.write(c, 4);

			o.write((char*)&s, sizeof(size_t));

			o.write((char*)_chunkSet[i]->buffer.getBuffer(), s);
		}

		o.close();
	}

	MemoryFileWriter* ChunkSet::getCurrentMemoryFileWriter()
	{
		return &_currentChunk->buffer;
	}
}