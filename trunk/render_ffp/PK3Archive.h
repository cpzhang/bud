//==========================================================================
/**
* @file	  : PK3Archive.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-18   19:47
* purpose : 
*/
//==========================================================================

#ifndef __PK3Archive_h__
#define __PK3Archive_h__

#include "Common.h"

namespace Euclid
{
	class PK3Archive
	{
	public:
		PK3Archive();
		~PK3Archive();

	public:
		struct sLocalFileHeader 
		{
			sLocalFileHeader()
			{
				first_part = new sFirstPart;
				memset(first_part, 0, sizeof(sFirstPart));
				file_name = NULL;
				reader = NULL;
			}
			struct sFirstPart
			{
				sFirstPart()
				{
				}
				//	Local file header signature = 0x04034b50
				static const unsigned int	SIGNATURE;
				unsigned int				signature;
				//	Version needed to extract (minimum)
				signed short				version;
				//	General purpose bit flag
				signed short				flag;
				//	Compression method
				signed short				compression_method;
				//	File last modification time
				signed short				last_modification_time;
				//	File last modification date
				signed short				lat_modification_date;
				//	CRC-32
				unsigned int				crc32;
				//	Compressed size
				unsigned int				compressed_size;
				//	Uncompressed size
				unsigned int				uncompressed_size;
				//	File name length (n)
				signed short				file_name_length;
				//	Extra field length (m)
				signed short				extra_field_length;
			};

			sFirstPart*			first_part;
			//	File name
			char*				file_name;
			//	Extra field
			//m bytes

			//	Added by cp
			unsigned int		offset;
			IFileRead*			reader;
		};

		//	ZIP end of central directory record
		struct sEnd_of_Central_Directory_Record 
		{
			//	End of central directory signature = 0x06054b50
			unsigned int	signature; 
			// Number of this disk
			unsigned short	number_of_this_disk; 
			//	Disk where central directory starts
			unsigned short	disk_where_central_directory_starts; 
			// Number of central directory records on this disk
			unsigned short	number_of_central_directory_records_on_this_disk; 
			//	Total number of central directory records
			unsigned short  total_number_of_central_directory_records; 
			//	Size of central directory (bytes)
			unsigned int size_of_central_directory;                                                                                                                                              
			//	Offset of start of central directory, relative to start of archive
			unsigned int offset_of_start_of_central_directory;
			//	ZIP file comment length (n)
			unsigned short comment_length;
			//ZIP file comment
		};

	public:
		bool create(const std::string& path);
		IFileRead* getFileReader(const std::string& name);
	private:
		typedef std::map<std::string, sLocalFileHeader*> FileMap;
		FileMap								_fileMap;
		std::string							_path;
		//sEnd_of_Central_Directory_Record*	_end;
	};
}

#endif // __PK3Archive_h__
 
