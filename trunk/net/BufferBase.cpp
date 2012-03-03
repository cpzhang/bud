#include "BufferBase.h"
namespace Shannon
{
	// The value that will be used if setMaxExpectedBlockSize() is not called (or called with a negative argument)
	u32 BufferBase::DefaultMaxExpectedBlockSize = 1048576; // 10 M unless changed by a NeL variable

	// The value that will be used if setMaxSentBlockSize() is not called (or called with a negative argument)
	u32 BufferBase::DefaultMaxSentBlockSize = 1048576; // 10 M unless changed by a NeL variable

}