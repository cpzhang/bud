#include "Exception.h"

namespace Zen
{
	bool IsRealEqual(const float& a, const float& b)
	{
		if (a > b)
		{
			return (a - b) <= 0.0001f;
		}
		return (b - a) <= 0.0001f;
	}

	Exception::Exception()
		:_reason("unkonwn exception")
	{

	}

	Exception::Exception( const std::string &reason )
		:_reason(reason)
	{

	}

	Exception::Exception( const char *format, ... )
	{
		std::string str;
		Convert_Vargs2String(str, format, MaxCStringSize);
	}

	const char	* Exception::what() const throw()
	{
		return _reason.c_str();
	}
}
