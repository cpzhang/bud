//==========================================================================
/**
* @file	  : Wstring.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-20   14:42
* purpose : 
*/
//==========================================================================

#ifndef __wstring_h__
#define __wstring_h__

#include "Common.h"

namespace Zen
{
	class _BUHHDA_EXPORT_ Wstring : public std::string
	{
	public:
		Wstring();
		Wstring(const char* pch);
		Wstring(const std::string& str);
		Wstring(const Wstring& wstr);
		~Wstring();
	public:
		wchar_t* c_wstr();
	private:
		wchar_t*	_wchar;
	};
}
#endif // __wstring_h__
