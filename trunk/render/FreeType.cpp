#include "FreeType.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ITexture.h"
//
namespace Euclid
{
	FreeType::FreeType(Vec3 a)
		: _activeTex(0), _baseX(0), _renderSystem(0)
	{
		
	}

	FreeType::~FreeType()
	{

	}

	bool FreeType::create( std::string& faceFile, unsigned int fontSize, eFontProperty fontProperty )
	{
		//
		if (FT_Init_FreeType(&_library) != 0)
		{
			return false;
		}

		//
		if (FT_New_Face(_library, faceFile.c_str(), 0, &_face) != 0)
		{
			return false;
		}

		//
		if (FT_Set_Pixel_Sizes(_face, 0, fontSize) != 0)
		{
			return false;
		}

		//
		//
		_property = fontProperty;
		_fontSize = fontSize;
		_OFFSET_VERTICAL = (unsigned int)(_fontSize * 1.2) + 2;
		//
		_renderSystem = RenderSystem::getInstancePtr();
		//
		return true;
	}


	bool FreeType::destroy()
	{
		return true;
	}	


	bool FreeType::render( Vec3& basePoint, Vec3& direction, const Color& color, std::string& text )
	{
		if (text.size() == 0)
		{
			return true;
		}
		if (NULL == _renderSystem)
		{
			return false;
		}

		//
		unsigned short unicode = 0;
		FTex* fTex = 0;
		_baseX = 0;
		unsigned int spaceOffset = _fontSize * 0.5;
		basePoint.y += _fontSize;
		//
		for (size_t i = 0; i < text.size(); ++i)
		{
			if (text[i] < 0)
			{
				unicode = _computeUnicode(text.substr(i, 2));
				++i;
			}
			else
			{
				unicode = _computeUnicode(text.substr(i, 1));
			}

			//
			fTex = _parse(unicode);
			if (NULL == fTex)
			{
				basePoint.x += spaceOffset;
			}
			else
			{
				_renderImpl(fTex, color, basePoint);
			}

			//
			unicode = 0;
			fTex = 0;
		}

		//
		return true;
	}

	void FreeType::_renderImpl( FTex* fft, const Color& color, Vec3& direction )
	{
		//
		if (NULL == _renderSystem)
		{
			return;
		}

		//
		if (!_renderSystem->setTexture(0, fft->_tex))
		{
			return;
		}

		//
		//static unsigned short i[6] = {0, 1, 3, 1, 2, 3};
		static unsigned short i[6] = {0, 1, 2, 0, 2, 3};

	
		// 
		_renderSystem->setVertexDeclaration(eVertexDeclarationType_PositionColorTexture);

		//
		sPositionColorTexture vertices[4]; 
		vertices[0].position = Vec3(_baseX + fft->_bearingX, fft->_bearingY, 0.0f);
		vertices[0].color_ARGB = color.getARGB();
		vertices[0].texcoord = fft->_uv0;

		vertices[1].position = Vec3(_baseX + fft->_bearingX + fft->_width, fft->_bearingY,	0);
		vertices[1].color_ARGB = color.getARGB();
		vertices[1].texcoord = Vec2(fft->_uv2.x, fft->_uv0.y);

		vertices[2].position = Vec3(_baseX + fft->_bearingX + fft->_width,	-fft->_height + fft->_bearingY, 0);
		vertices[2].color_ARGB = color.getARGB();
		vertices[2].texcoord = fft->_uv2;

		vertices[3].position = Vec3(_baseX + fft->_bearingX, -fft->_height + fft->_bearingY, 0);
		vertices[3].color_ARGB = color.getARGB();
		vertices[3].texcoord = Vec2(fft->_uv0.x, fft->_uv2.y);
		
		//
		_renderSystem->drawIndexedPrimitiveUP(ePrimitive_TriangleList, 0, 4, 2, i, eFormat_Index16, vertices, sizeof(sPositionColorTexture));

		//
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setVertexDeclaration(eVertexDeclarationType_Null);

		//
		_baseX += fft->_advance;
	}

	unsigned short FreeType::_computeUnicode( std::string& character)
	{
		wchar_t wc = 0;
		::MultiByteToWideChar(CP_ACP, 0, character.c_str(), -1, &wc, 1);

		//
		return wc;
	}

	FTex* FreeType::_parse( unsigned short unicode )
	{
		CodeTexMap::iterator it = _codeTex.find(unicode);
		if (it == _codeTex.end())
		{
			_addCode(unicode);
		}
		return _codeTex[unicode];
	}

	void FreeType::_addCode( unsigned short unicode )
	{
		//
		if (FT_Load_Char(_face, unicode, FT_LOAD_DEFAULT | FT_LOAD_RENDER 	|
			FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT))
		{
			return;
		}

		//
		if ( _face->glyph->bitmap.buffer == 0)
		{
			return;
		}

		//
		FT_GlyphSlot slot = _face->glyph;
		FT_Bitmap bitmap = slot->bitmap;
		
		//
		FTex* fft = new FTex;
		fft->_width = slot->metrics.width >> 6;
		fft->_height = slot->metrics.height >> 6;
		fft->_bearingX = slot->metrics.horiBearingX >> 6;
		fft->_bearingY = slot->metrics.horiBearingY >> 6;
		fft->_advance = slot->metrics.horiAdvance >> 6;

		//
		unsigned int* buffer = 0;
		unsigned char alphaAddon = 0;
		switch(_property)
		{
			//
		case eFontProperty_Normal:
		default:
			{
				buffer = new unsigned int[bitmap.width * bitmap.rows];
				memset(buffer, 0, sizeof(unsigned int) * bitmap.width * bitmap.rows);
				switch (bitmap.pixel_mode)
				{
				case FT_PIXEL_MODE_MONO:
					{
						for (int i = 0; i < bitmap.rows; ++i)
						{
							unsigned char *src = bitmap.buffer + (i * bitmap.pitch);
							unsigned int pitch = i * bitmap.rows;
							int j;
							for (j = 0; j < bitmap.width; ++j)
								buffer [j + pitch] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
						}
					}
					break;
				case FT_PIXEL_MODE_GRAY:
					{
						//
						for (int i = 0; i < bitmap.rows; ++i)
							for (int j = 0; j < bitmap.width; ++j)
							{
								unsigned char c = bitmap.buffer[i * bitmap.pitch + j];
								if(c > 0)
									buffer[i * bitmap.width + j] = (c << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
							}
					}
				}
			}	
			break;
		}

		//
		if (NULL == _activeTex)
		{
			_activeTex = TextureManager::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, D3DFMT_A8R8G8B8);
			if (NULL == _activeTex)
			{
				return;
			}

			//
			_pen.x = 0;
			_pen.y = 0;
		}

		//
		if (_pen.x + fft->_width > _TEXTURE_SIZE)
		{
			_pen.x = 0;
			_pen.y += _OFFSET_VERTICAL;
			if (_pen.y + _OFFSET_VERTICAL > _TEXTURE_SIZE)
			{
				_activeTex = TextureManager::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, D3DFMT_A8R8G8B8);
				if (NULL == _activeTex)
				{
					return;
				}

				//
				_pen.y = 0;
			}
			else
			{
			}
		}

		//
		_activeTex->setSubData(0, (unsigned int)_pen.x, (unsigned int)_pen.y,
				fft->_width, fft->_height, 4 * fft->_width,
				buffer, D3DFMT_A8R8G8B8);

		//
		if (buffer)
		{
			delete[] buffer;
			buffer = 0;
		}

		//
		fft->_uv0 = Vec2(_pen.x * _INVERSE_TEXTURE_SIZE, _pen.y * _INVERSE_TEXTURE_SIZE);
		
		//
		//_pen.y += 2;

		//
		fft->_uv2 = Vec2((_pen.x + fft->_width)  * _INVERSE_TEXTURE_SIZE, (_pen.y + fft->_height) * _INVERSE_TEXTURE_SIZE);
		fft->_tex = _activeTex;
		_codeTex[unicode] = fft;

		//
		_pen.x += fft->_width;
		_pen.x += 2;
	}

	const unsigned int FreeType::_TEXTURE_SIZE(256);
	const float FreeType::_INVERSE_TEXTURE_SIZE(1.0f / _TEXTURE_SIZE);
}
