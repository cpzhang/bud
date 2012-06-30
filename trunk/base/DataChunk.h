/********************************************************************
**	file: 		DataChunk.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-6-11
**	purpose:	
*********************************************************************/
#ifndef __DataChunk_h__
#define __DataChunk_h__
#include "Common.h"

namespace Buddha
{
	class MemoryFileWriter
	{
	public:
		MemoryFileWriter();
		~MemoryFileWriter();
	public:
		bool isEnd();

		bool isOverFlow(size_t s);

		bool reallocate(size_t s);

		bool write(void* b, size_t s);

		bool _writeImp(void* b, size_t s);

		size_t getSize();

		void* getBuffer();
	private:
		void*			_buffer;
		unsigned int	_cursor;
		unsigned int	_length;
	};

	struct sChunk
	{
	public:
		sChunk()
		{
		}
		~sChunk()
		{
		}
		std::string name;
		MemoryFileWriter buffer;
	};

	class _BUHHDA_EXPORT_ ChunkSet
	{
	public:
		std::vector<sChunk*> _chunkSet;
		sChunk*			_currentChunk;

	public:
		ChunkSet();
		~ChunkSet();
	public:
		bool beginChunk(const std::string& name);

		void endChunk();

		bool write(const char* b, size_t s);

		bool write(void* b, size_t s);

		void save(const std::string& fileName);

		MemoryFileWriter* getCurrentMemoryFileWriter();
	};
}
#endif // __DataChunk_h__
