//==========================================================================
/**
* @file	  : FontManager.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   16:25
* purpose : 
*/
//==========================================================================

#ifndef __FontManager_h__
#define __FontManager_h__

//
#include "Common.h"
#include "FreeType.h"
//
namespace Zen
{
	//
	class _ZenExport_ FontManager :public Zen::SingletonEx<FontManager>
	{
		//
	public:
		//
		bool createFont(std::string& faceFile, unsigned int fontSize, eFontProperty fp, std::string fontName);
		
		//
		FreeType* getFont(const std::string& fontName);

		//
		void destroy();
		void onInvalidateDevice();
		void onRestoreDevice();
	public:
		FontManager();
		~FontManager();

	private:
		//
		typedef std::map<std::string, FreeType*> NameFreetypeMap;
		NameFreetypeMap				_fonts;
	};
}

#endif // __FontManager_h__
