//
#include "Wstring.h" 

namespace Buddha
{
	Wstring::Wstring()
		:	std::string(),
			_wchar(0)
	{
	}

	Wstring::Wstring( const char* pch )
		:	std::string(pch),
			_wchar(0)
	{
	}

	Wstring::Wstring( const std::string& str )
		:	std::string(str),
			_wchar(0)
	{
	}

	Wstring::Wstring( const Wstring& wstr )
		:	std::string(wstr.c_str()),
			_wchar(0)
	{

	}
	Wstring::~Wstring()
	{
		if (_wchar)
		{
			delete[] _wchar;
			_wchar = 0;
		}
	}

	wchar_t* Wstring::c_wstr()
	{
		if (_wchar)
		{
			delete[] _wchar;
			_wchar = 0;
		}
		_wchar = new wchar_t[this->size()];
		::MultiByteToWideChar(CP_ACP, 0, this->c_str(), -1, _wchar, this->size());
		//
		return _wchar;
	}
}