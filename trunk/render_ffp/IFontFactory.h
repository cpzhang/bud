//==========================================================================
/**
* @file	  : IFontFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   17:08
* purpose : 
*/
//==========================================================================

#ifndef __IFontFactory_h__
#define __IFontFactory_h__

//
#include "Common.h"
#include "IFont.h"

namespace Euclid
{
	//
	enum FontType
	{
		FontType_FreeType,
		FontType_Windows
	};

	//
	class IFontFactory
	{
	public:
	public:
		//
		virtual bool createFont(FontType ft, std::string& faceFile, unsigned int fontSize, FontProperty fp, std::string fontName) = 0;

		//
		virtual IFont* getFont(std::string& fontName = std::string("")) = 0;
	};
}

#endif // __IFontFactory_h__
