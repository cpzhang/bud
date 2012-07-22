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
#include "Color.h"
//
#include "external/freetype-2.4.4/include/ft2build.h"
#include FT_FREETYPE_H

//
namespace Zen
{
	//
	enum eFontProperty
	{
		//
		eFontProperty_Normal = 0,
		eFontProperty_Offset_1,
		eFontProperty_BG_1,
		eFontProperty_Size,
	};

	//
	struct FTex
	{
		FTex()
			:_tex(NULL)
		{

		}
		ITexture*		_tex;
		Vec2			_uv0;
		Vec2			_uv2;
		Real			_advance;
		Real			_bearingX;
		Real			_bearingY;
		Real			_width;
		Real			_height;
	};

	class _ZenExport_ FreeType
	{
	public:
		FreeType(Vec3);
		~FreeType();

	public:
		virtual bool create(std::string& faceFile, unsigned int fontSize, eFontProperty fontProperty);
		virtual bool destroy();
		virtual bool render(Vec3& basePoint, Vec3& direction, const Color& color, std::string& text);
		void onInvalidateDevice();
		void onRestoreDevice();
	private:
		//
		unsigned short _computeUnicode(std::string& character);
		FTex* _parse(unsigned short unicode, bool chinese = false);
		void _addCode(unsigned short unicode, bool chinese = false);
		void _renderImpl(FTex* fft, const Color& color, Vec3& direction);

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
		eFontProperty	_property;

		//
		typedef std::map<unsigned long, FTex*> CodeTexMap;
		CodeTexMap		_codeTex;

		//
		IRenderSystem*	_renderSystem;
		ITexture*		_activeTex;
		Vec2			_pen;
		Real			_baseX;
		Effect*			_fx;
		static ITexture* _diffuseTexture;
	};
}

#endif // __FreeType_h__
 
