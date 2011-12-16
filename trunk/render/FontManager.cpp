#include "FontManager.h"
#include "FreeType.h"

//
namespace Euclid
{
	FontManager::FontManager()
	{

	}

	FontManager::~FontManager()
	{

	}

	bool FontManager::createFont(std::string& faceFile, unsigned int fontSize, eFontProperty fp, std::string fontName )
	{
		//
		FreeType* font = new FreeType(Vec3::ZERO);
		if (font->create(faceFile, fontSize, fp))
		{
			_fonts[fontName] = font;
		}
		else
		{
			return false;
		}
	
		//
		return (font != 0);
	}

	FreeType* FontManager::getFont( std::string& fontName /*= ""*/ )
	{
		if (fontName.size() == 0)
		{
			return _fonts.begin()->second;
		}
		return _fonts[fontName];
	}

}
