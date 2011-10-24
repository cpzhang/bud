//==========================================================================
/**
* @file	  : IFont.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   16:09
* purpose : 
*/
//==========================================================================

#ifndef __IFont_h__
#define __IFont_h__

#include "stdafx.h"
#include "Common.h"

//
namespace Euclid
{
	enum FontDimention
	{
		//
		FontDimention_2D,

		//
		FontDimention_3D
	};
	//
	enum FontProperty
	{
		//
		FONT_PROP_NONE,

		//
		FONT_PROP_PROJECT_1,

		//
		FONT_PROP_PROJECT_2,

		//
		FONT_PROP_OUTLINE,

		//
		FONT_PROP_BG_1,

		//
		FONT_PROP_OFFSET_1,
	};

	class IFont
	{
	public:
		virtual bool create(std::string& faceFile, unsigned int fontSize, FontProperty fontProperty) = 0;
		virtual bool render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, std::string& text) = 0;
		virtual bool render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, const char* text) = 0;
		virtual bool destroy() = 0;
	};
}

#endif // __IFont_h__
 
