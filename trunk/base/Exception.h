/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:21
*********************************************************************/
#ifndef __Exception_h__
#define __Exception_h__
#include "Common.h"
namespace Zen
{
	bool _BUHHDA_EXPORT_ IsRealEqual(const float& a, const float& b);

	class _BUHHDA_EXPORT_ Exception : public std::exception
	{
	protected:
		std::string	_reason;
	public:
		Exception();
		Exception(const std::string &reason);
		Exception(const char *format, ...);
		virtual ~Exception() throw() {}
		virtual const char	*what() const throw();
	};
	/*
	 * Thread exception
	 */
	class EThread : public Exception
	{
	public:
		EThread (const char* message) : Exception (message) {}
	};
}

#endif //__Exception_h__
