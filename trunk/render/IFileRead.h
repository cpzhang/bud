//==========================================================================
/**
* @file	  : IFileRead.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-20   10:50
* purpose : 
*/
//==========================================================================

#ifndef __IFileRead_h__
#define __IFileRead_h__

#include "Common.h"

namespace Euclid
{
	class IFileRead
	{
	public:
		virtual unsigned int read(void* buff, unsigned int size) = 0;
		//! Changes position in file
		/** \param finalPos Destination position in the file.
		\param relativeMovement If set to true, the position in the file is
		changed relative to current position. Otherwise the position is changed
		from beginning of file.
		\return True if successful, otherwise false. */
		virtual bool seek(long step, bool relative2CurrentCursor = true) = 0;

		virtual bool seek2Begin() = 0;

		//! Get the current position in the file.
		/** \return Current position in the file in bytes. */
		virtual long getCursor() const = 0;

		//! returns size of file
		virtual long getSize() const = 0;

		//
		virtual bool isEnd() const = 0;

		//
		virtual u32 getUnreadSize() const = 0;
	};
}

#endif // __IFileRead_h__
 
