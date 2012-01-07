/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:21
*********************************************************************/
#ifndef __Exception_h__
#define __Exception_h__
#include "Common.h"
namespace Buddha
{
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
}

#endif //__Exception_h__
