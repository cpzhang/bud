#include "PK3Archive.h"
#include "external/zlib/zlib.h"
#include "MemoryFileReader.h"

namespace Euclid
{
	PK3Archive::PK3Archive()
	{

	}

	PK3Archive::~PK3Archive()
	{

	}

	bool PK3Archive::create(const std::string& path )
	{
		std::string data;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(data);
		data += path;
		

		_path = data;
		std::ifstream f(data.c_str(), std::ios_base::binary);

		//
		if (!f.good())
		{
			return false;
		}

		for (;;)
		{
			sLocalFileHeader* localFileHeader = new sLocalFileHeader;

			f.read((char*)localFileHeader->first_part, sizeof(sLocalFileHeader::sFirstPart));

			if (localFileHeader->first_part->signature != sLocalFileHeader::sFirstPart::SIGNATURE)
			{
				break;
			}

			signed short length = localFileHeader->first_part->file_name_length;

			localFileHeader->file_name = new char[length + 1];
			f.read(localFileHeader->file_name, length);
			localFileHeader->file_name[length] = 0;

			f.seekg(localFileHeader->first_part->extra_field_length, std::ios_base::cur);

			localFileHeader->offset = f.tellg();
			f.seekg(localFileHeader->first_part->compressed_size, std::ios_base::cur);

			std::string filePath(localFileHeader->file_name);
			int dot_pos = filePath.find('.');
			if (dot_pos != std::string::npos)
			{
				int start_pos = filePath.find_last_of('/');
				if (start_pos != std::string::npos)
				{
					++start_pos;
				}
				else
				{
					start_pos = 0;
				}
				filePath = filePath.substr(start_pos, filePath.size());
				//dot_pos = filePath.find('.');
				//filePath = filePath.substr(0, dot_pos);
				if (_fileMap.find(filePath) == _fileMap.end())
				{
					_fileMap[filePath] = localFileHeader;
				}
				else
				{
					//	file with that name has already existed.
				}
			}
		}

		f.close();

		return true;
	}

	IFileRead* PK3Archive::getFileReader( const std::string& name )
	{
		if (_fileMap.find(name) == _fileMap.end())
		{
			return NULL;
		}

		if (NULL == _fileMap[name]->reader)
		{
			const unsigned int uncompressed_size = _fileMap[name]->first_part->uncompressed_size;
			const unsigned int compressed_size = _fileMap[name]->first_part->compressed_size;

			void* uncompressed_buffer = new char[uncompressed_size];
			char* compressed_buffer = new char[compressed_size];	

			std::ifstream f(_path.c_str(), std::ios_base::binary);

			//
			if (!f.good())
			{
				return NULL;
			}

			f.seekg(_fileMap[name]->offset);

			f.read(compressed_buffer, compressed_size);

			// Setup the inflate stream.
			z_stream stream;
			int err;
			stream.next_in = (Bytef*)compressed_buffer;
			stream.avail_in = compressed_size;
			stream.next_out = (Bytef*)uncompressed_buffer;
			stream.avail_out = uncompressed_size;
			stream.zalloc = (alloc_func)0;
			stream.zfree = (free_func)0;

			// Perform inflation. wbits < 0 indicates no zlib header inside the data.
			err = inflateInit2(&stream, -MAX_WBITS);
			if (err == Z_OK)
			{
				err = inflate(&stream, Z_FINISH);
				inflateEnd(&stream);
				if (err == Z_STREAM_END)
					err = Z_OK;
				err = Z_OK;
				inflateEnd(&stream);
			}

			delete compressed_buffer;

			if (err == Z_OK)
			{
				_fileMap[name]->reader = new MemoryFileReader(uncompressed_buffer, uncompressed_size);
			}
			f.close();

		}

		return _fileMap[name]->reader;
	}

	const unsigned int PK3Archive::sLocalFileHeader::sFirstPart::SIGNATURE(0x04034b50);

}
