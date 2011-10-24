#include "stdafx.h"
#include "FontFactory.h"
#include "FreeType.h"

//
namespace Euclid
{


	FontFactory::FontFactory()
	{

	}

	FontFactory::~FontFactory()
	{

	}

	bool FontFactory::createFont(FontType ft, std::string& faceFile, unsigned int fontSize, FontProperty fp, std::string fontName )
	{
		//
		IFont* font = 0;
		//
		switch(ft)
		{
		case FontType_Windows:
			//
			break;

		case FontType_FreeType:
			//
			font = new FreeType;
			if (font->create(faceFile, fontSize, fp))
			{
				_fonts[fontName] = font;
			}
			else
			{
				return false;
			}
			break;
		default:
			break;
		}
		//
		return (font != 0);
	}

	IFont* FontFactory::getFont( std::string& fontName /*= ""*/ )
	{
		if (fontName.size() == 0)
		{
			return _fonts.begin()->second;
		}
		return _fonts[fontName];
	}
}
