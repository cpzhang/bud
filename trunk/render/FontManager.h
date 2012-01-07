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
namespace Euclid
{
	//
	class _EuclidExport_ FontManager :public Buddha::SingletonEx<FontManager>
	{
		//
	public:
		//
		bool createFont(std::string& faceFile, unsigned int fontSize, eFontProperty fp, std::string fontName);
		
		//
		FreeType* getFont(std::string& fontName = std::string(""));

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
