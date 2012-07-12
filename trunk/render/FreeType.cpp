#include "FreeType.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ITexture.h"
#include "EffectManager.h"
#include "Effect.h"
#include "VDTManager.h"
//
namespace Euclid
{
	FreeType::FreeType(Vec3 a)
		: _activeTex(0), _baseX(0), _renderSystem(0), _fx(0)
	{
		
	}

	FreeType::~FreeType()
	{

	}

	bool FreeType::create( std::string& faceFile, unsigned int fontSize, eFontProperty fontProperty )
	{
		int err = 0;
		//
		err = FT_Init_FreeType(&_library);
		if (err != 0)
		{
			return false;
		}

		//
		err = FT_New_Face(_library, faceFile.c_str(), 0, &_face);
		if (err != 0)
		{
			std::string data = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
			data += faceFile;
			err = FT_New_Face(_library, data.c_str(), 0, &_face);
			if (err != 0)
			{
				return false;
			}
		}

		//
		//if (FT_Set_Pixel_Sizes(_face, 0, fontSize) != 0)
		//这种方式较上面的更为清晰
		if (FT_Set_Char_Size (_face, 0, FT_F26Dot6 (fontSize * 64), 96, 96) != 0)
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
		onRestoreDevice();
		
		//
		return true;
	}


	bool FreeType::destroy()
	{
		if (_activeTex)
		{
			_activeTex = 0;
		}
		if (_fx)
		{
			_fx->destroy();
//			delete _fx;
			_fx = 0;
		}
		for (CodeTexMap::iterator i = _codeTex.begin(); i != _codeTex.end(); ++i)
		{
			if (i->second)
			{
				ITexture* t = i->second->_tex;
				if (t)
				{
					t->release();
					t = 0;
				}
			}
		}
		_codeTex.clear();
		//
		if (_diffuseTexture)
		{
			_diffuseTexture->release();
			_diffuseTexture = NULL;
		}
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
		_baseX = basePoint.x;
		unsigned int spaceOffset = _fontSize * 0.5;
		basePoint.y += _fontSize;
		//
		for (size_t i = 0; i < text.size(); ++i)
		{
			bool chinese = false;
			if (text[i] < 0)
			{
				unicode = _computeUnicode(text.substr(i, 2));
				++i;
				chinese = true;
			}
			else
			{
				unicode = _computeUnicode(text.substr(i, 1));
			}

			//
			fTex = _parse(unicode, chinese);
			if (NULL == fTex)
			{
				// '\n'
				if (unicode == 10)
				{
					basePoint.y += _fontSize;
					_baseX = basePoint.x;
				} 
				else
				{
					_baseX += spaceOffset;
				}
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

	void FreeType::_renderImpl( FTex* fft, const Color& color, Vec3& basePoint )
	{
		static unsigned short sIndices[6] = {0, 1, 2, 0, 2, 3};
	
		eVertexDeclarationType vdt = eVertexDeclarationType_PositionTTexture;
		// 
		_renderSystem->setVertexDeclaration(vdt);
		//
		D3DVIEWPORT9 vp;
		RenderSystem::getInstancePtr()->getViewPort(&vp);
		//
		sPositionTTexture vertices[4];
		vertices[0].position = Vec4(_baseX + fft->_bearingX, basePoint.y - fft->_bearingY, 0.f, 1.0f);
		vertices[0].texcoord = fft->_uv0;

		vertices[1].position = Vec4(_baseX + fft->_bearingX + fft->_width, basePoint.y - fft->_bearingY,	0.f, 1.0f);
		vertices[1].texcoord = Vec2(fft->_uv2.x, fft->_uv0.y);

		vertices[2].position = Vec4(_baseX + fft->_bearingX + fft->_width,	basePoint.y + fft->_height - fft->_bearingY, 0.f, 1.0f);
		vertices[2].texcoord = fft->_uv2;

		vertices[3].position = Vec4(_baseX + fft->_bearingX, basePoint.y + fft->_height - fft->_bearingY, 0.f, 1.0f);
		vertices[3].texcoord = Vec2(fft->_uv0.x, fft->_uv2.y);
		//
		// render
		{
			size_t sz = VDTManager::getInstancePtr()->mLayouts[vdt]->size();
			//
			u32 passes = 0;
			_fx->setTexture("g_MeshTexture", fft->_tex);
			//_fx->setTexture("g_MeshTexture0", _diffuseTexture); 
			_fx->setFloatArray("g_diffuse", (float*)&(color.r), 4);
			_fx->begin(&passes);
			for (u32 i = 0; i != passes; ++i)
			{
				_fx->beginPass(i);
				//
				_renderSystem->drawIndexedPrimitiveUP(ePrimitive_TriangleList, 0, 4, 2, sIndices, eFormat_Index16, vertices, sz);
		
				_fx->endPass();
			}
			_fx->end();
		}
		
		//
		_renderSystem->setVertexDeclaration(eVertexDeclarationType_Null);

		//
		_baseX += fft->_width;
	}

	unsigned short FreeType::_computeUnicode( std::string& character)
	{
		wchar_t wc = 0;
		::MultiByteToWideChar(CP_ACP, 0, character.c_str(), -1, &wc, 1);

		//
		return wc;
	}

	FTex* FreeType::_parse( unsigned short unicode , bool chinese/* = false*/)
	{
		CodeTexMap::iterator it = _codeTex.find(unicode);
		if (it == _codeTex.end())
		{
			_addCode(unicode, chinese);
		}
		return _codeTex[unicode];
	}

	void FreeType::_addCode( unsigned short unicode , bool chinese /*= false*/)
	{
		//
		if (FT_Load_Char(_face, unicode,
			FT_LOAD_DEFAULT |
			FT_LOAD_MONOCHROME |
			FT_LOAD_TARGET_LIGHT |
			FT_LOAD_RENDER |
			FT_LOAD_TARGET_NORMAL |
			FT_LOAD_FORCE_AUTOHINT))
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
		//if (chinese)
		if(unicode > 255)
		{
			fft->_bearingX = 0;
			fft->_bearingY = slot->metrics.horiBearingY >> 6;
		} 
		else
		{
			fft->_bearingX = slot->metrics.horiBearingX >> 6;
			fft->_bearingY = slot->metrics.horiBearingY >> 6;
		}
		fft->_advance = slot->metrics.horiAdvance >> 6;
		u8* buffer = 0;

		unsigned char alphaAddon = 0;
		switch(_property)
		{
// 			
// 		case eFontProperty_BG_1:
// 			{
// 				fft->_width += 2;
// 				fft->_height -= 2;
// 			}
// 			break;
			//
		case eFontProperty_Offset_1:
			{
				fft->_bearingX += 1;
				fft->_bearingY -= 1;
			}
		case eFontProperty_Normal:
		default:
			{
				buffer = new u8[bitmap.width * bitmap.rows];
				memset(buffer, 0, sizeof(u8) * bitmap.width * bitmap.rows);
				switch (bitmap.pixel_mode)
				{
				case FT_PIXEL_MODE_MONO:
					{
						for (int i = 0; i < bitmap.rows; ++i)
						{
							unsigned char *src = bitmap.buffer + (i * bitmap.pitch);
							for (int j = 0; j < bitmap.width; ++j)
								buffer [j + i * bitmap.rows] = (src[j/8] & (0x80 >> (j & 7))) ? 0xFF : 0x00;
						}
					}
					break;
				case FT_PIXEL_MODE_GRAY:
					{
						//
						for (int i = 0; i < bitmap.rows; ++i)
							for (int j = 0; j < bitmap.width; ++j)
							{
									buffer[i * bitmap.width + j] = bitmap.buffer[i * bitmap.pitch + j];
							}
					}
				}
			}	
			break;
		}

		//
		if (NULL == _activeTex)
		{
			_activeTex = TextureManager::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, 1, eUsage_Null, eFormat_A8, ePool_Manager);
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
				_activeTex = TextureManager::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, 1, eUsage_Null, eFormat_A8, ePool_Manager);
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

		int offsetX = 0;
		int offsetY = 0;
		if (_property == eFontProperty_BG_1)
		{
			for (size_t x = 0; x <= 2; ++x)
			for (size_t y = 0; y <= 2; ++y)
			{
				_activeTex->setSubData(0, (unsigned int)_pen.x + x , (unsigned int)_pen.y + y, fft->_width, fft->_height, fft->_width, buffer, D3DFMT_A8);
			}
			//
			//fft->_width += 2.0f;
			//fft->_height += 2.0f;
			offsetY = 2;
			offsetX = 2;
			//fft->_bearingX -= 1;
			//fft->_bearingY -= 1;
		}
		else
		{
			//
			_activeTex->setSubData(0, (unsigned int)_pen.x, (unsigned int)_pen.y,
				fft->_width, fft->_height, fft->_width,
				buffer, D3DFMT_A8);
		}
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
		fft->_uv2 = Vec2((_pen.x + fft->_width + offsetX)  * _INVERSE_TEXTURE_SIZE, (_pen.y + fft->_height + offsetY) * _INVERSE_TEXTURE_SIZE);
		fft->_tex = _activeTex;
		_codeTex[unicode] = fft;
		_activeTex->addReference();

		//
		_pen.x += (fft->_width + offsetX);
		_pen.x += 2;
	}

	void FreeType::onInvalidateDevice()
	{
		if (_activeTex)
		{
			_activeTex = 0;
		}
		_baseX = 0;
		_pen.x = 0;
		_pen.y = 0;

		for (CodeTexMap::iterator i = _codeTex.begin(); i != _codeTex.end(); ++i)
		{
			if (i->second)
			{
				ITexture* t = i->second->_tex;
				if (t)
				{
					t->release();
					t = 0;
				}
			}
		}
		_codeTex.clear();
	}

	void FreeType::onRestoreDevice()
	{
		_fx = EffectManager::getInstancePtr()->createEffectFromFile("shader\\Freetype.fx");
		if (NULL == _diffuseTexture)
		{
			_diffuseTexture = TextureManager::getInstancePtr()->createTextureFromFile("image/font.jpg");
		}
	}

	ITexture* FreeType::_diffuseTexture(NULL);

	const unsigned int FreeType::_TEXTURE_SIZE(256);
	const float FreeType::_INVERSE_TEXTURE_SIZE(1.0f / _TEXTURE_SIZE);
}
