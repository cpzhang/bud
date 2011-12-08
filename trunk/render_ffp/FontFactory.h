//==========================================================================
/**
* @file	  : FontFactory.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   16:25
* purpose : 
*/
//==========================================================================

#ifndef __FontFactory_h__
#define __FontFactory_h__

//
#include "Common.h"
#include "IFontFactory.h"

//
namespace Euclid
{
	//
	class FontFactory : public IFontFactory, public Buddha::SingletonEx<FontFactory>
	{
	public:
		FontFactory();
		~FontFactory();
		//
	public:
		//
		bool createFont(FontType ft, std::string& faceFile, unsigned int fontSize, FontProperty fp, std::string fontName);
		
		//
		IFont* getFont(std::string& fontName = std::string(""));

	private:
		//
		std::map<std::string, IFont*>		_fonts;
	};
}

#endif // __FontFactory_h__
