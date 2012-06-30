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
		destroy();
	}

	bool FontManager::createFont(tstring& faceFile, unsigned int fontSize, eFontProperty fp, tstring fontName )
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

	FreeType* FontManager::getFont( const tstring& fontName)
	{
		if (fontName.size() == 0)
		{
			return _fonts.begin()->second;
		}
		return _fonts[fontName];
	}

	void FontManager::destroy()
	{
		for (NameFreetypeMap::iterator i = _fonts.begin(); i != _fonts.end(); ++i)
		{
			FreeType* ft = i->second;
			if (ft)
			{
				ft->destroy();
				delete ft;
				ft = 0;
			}
		}
		_fonts.clear();
	}

	void FontManager::onInvalidateDevice()
	{
		for (NameFreetypeMap::iterator i = _fonts.begin(); i != _fonts.end(); ++i)
		{
			FreeType* ft = i->second;
			if (ft)
			{
				ft->onInvalidateDevice();
			}
		}
	}

	void FontManager::onRestoreDevice()
	{
		for (NameFreetypeMap::iterator i = _fonts.begin(); i != _fonts.end(); ++i)
		{
			FreeType* ft = i->second;
			if (ft)
			{
				//ft->onRestoreDevice();
			}
		}
	}

}
