//==========================================================================
/**
* @file	  : FreeType.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   16:08
* purpose : 
*/
//==========================================================================

#ifndef __FreeType_h__
#define __FreeType_h__

//
#include "Common.h"
#include "IFont.h"
#include "IRenderSystem.h"
#include "ITexture.h"

//
#include "external/freetype-2.4.4/include/ft2build.h"
#include FT_FREETYPE_H

//
namespace Euclid
{
	//
	struct FTex
	{
		ITexture*		_tex;
		Vec2		_uv0;
		Vec2		_uv2;
		Real				_advance;
		Real				_bearingX;
		Real				_bearingY;
		Real				_width;
		Real				_height;
	};

	class FreeType : public IFont
	{
	public:
		FreeType();
		~FreeType();

	public:
		virtual bool create(std::string& faceFile, unsigned int fontSize, FontProperty fontProperty);
		virtual bool destroy();
		virtual bool render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, std::string& text);
		virtual bool render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, const char* text);

	private:
		//
		unsigned short _computeUnicode(std::string& character);
		FTex* _parse(unsigned short unicode);
		void _addCode(unsigned short unicode);
		void _renderImpl(FTex* fft, const D3DCOLOR& color, D3DXVECTOR3& direction);
		void _renderImpl2D(FTex* fft, const D3DCOLOR& color, D3DXVECTOR3& basePoint);

		//
	private:
		//
		static const unsigned int	_TEXTURE_SIZE;
		unsigned int				_OFFSET_VERTICAL;
		static const float			_INVERSE_TEXTURE_SIZE;
		//
	private:
		FT_Library		_library;
		FT_Face			_face;
		unsigned int	_fontSize;
		FontProperty	_property;

		//
		typedef std::map<unsigned long, FTex*> CodeTexMap;
		CodeTexMap		_codeTex;

		//
		IRenderSystem*	_renderSystem;
		ITexture*		_activeTex;
		D3DXVECTOR2		_pen;
		Real	_baseX;

		//
		FontDimention	_fontDimention;
	};
}

#endif // __FreeType_h__
 
