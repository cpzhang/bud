//==========================================================================
/**
* @file	  : MPQLoader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-24   21:11
* purpose : 
  references : 
  http://wiki.devklog.net/index.php?title=The_MoPaQ_Archive_Format
*/
//==========================================================================

#ifndef __MPQLoader_h__
#define __MPQLoader_h__

#include "Common.h"

namespace Euclid
{
	class MPQLoader
	{
	public:
		MPQLoader();
		~MPQLoader();

	public:
		struct sHeader
		{
			//	Indicates that the file is a MoPaQ archive. Must be ASCII "MPQ" 1Ah.
			char	magic[4];

			//	Size of the archive header
			int		header_size;

			//	Size of the whole archive, including the header. Does not include the strong digital signature, 
			//	if present. This size is used, among other things, for determining the region to hash in computing 
			//	the digital signature. This field is deprecated in the Burning Crusade MoPaQ format, and the size 
			//	of the archive is calculated as the size from the beginning of the archive to the end of the 
			//	hash table, block table, or extended block table (whichever is largest).
			int		archive_size;

			//	MoPaQ format version. MPQAPI will not open archives where this is negative. Known versions:
			//	0000h                  Original format. HeaderSize should be 20h, and large archives are not supported.
			//	0001h                  Burning Crusade format. Header size should be 2Ch, and large archives are supported.
			s16		format_version;

			//	Power of two exponent specifying the number of 512-byte disk sectors in each logical sector 
			//	in the archive. The size of each logical sector in the archive is 512 * 2^SectorSizeShift. 
			s8		sector_size_shift;

			//	Offset to the beginning of the hash table, relative to the beginning of the archive.
			s32		hash_table_offset;

			//	Offset to the beginning of the block table, relative to the beginning of the archive.
			s32		block_table_offset;

			//	Number of entries in the hash table. Must be a power of two, and must be less than 2^16 
			//	for the original MoPaQ format, or less than 2^20 for the Burning Crusade format.
			s32		hash_table_entries;

			//		Number of entries in the block table.
			s32		block_table_entries;

			//	Offset to the beginning of the extended block table, relative to the beginning of the archive
			s64		extended_block_table_offset;

			//	High 16 bits of the hash table offset for large archives.
			s16		hash_table_offset_high;

			//	High 16 bits of the block table offset for large archives.
			s16		block_table_offset_high;
		};

	public:
		bool load(const std::string& name);

	private:
		sHeader		_header;
	};
}

#endif // __MPQLoader_h__
 
