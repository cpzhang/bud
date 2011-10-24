#include "FreeType.h"
#include "TextureFactory.h"
#include "Paras.h"
//
namespace Euclid
{
	FreeType::FreeType()
		: _activeTex(0), _baseX(0), _renderSystem(0), _fontDimention(FontDimention_2D)
	{
		
	}

	FreeType::~FreeType()
	{

	}

	bool FreeType::create( std::string& faceFile, unsigned int fontSize, FontProperty fontProperty )
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
		_renderSystem = Paras::getInstancePtr()->_renderSystem;
		//
		return true;
	}


	bool FreeType::destroy()
	{
		return true;
	}	

	bool FreeType::render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, std::string& text )
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
		_fontDimention = fd;

		//
		switch(_fontDimention)
		{
			//
		case FontDimention_2D:
			{
// 				if (_renderSystem->isPerspectiveProjectionMode())
// 				{
// 					_renderSystem->switchProjectionMode();
// 				}
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
						_renderImpl2D(fTex, color, basePoint);
					}

					//
					unicode = 0;
					fTex = 0;
				}

				//
// 				if (_renderSystem->isOrthographicProjectionMode())
// 				{
// 					_renderSystem->switchProjectionMode();
// 				}
			}
			break;

			//
		case FontDimention_3D:
			{
				//
				D3DXMATRIX mx;
				if (!_renderSystem->getWorldMatrix(&mx))
				{
					return false;
				}

				D3DXMATRIX cur;
				D3DXMATRIX tran;
				D3DXMATRIX rota;
				D3DXMatrixTranslation(&tran, basePoint.x, basePoint.y, basePoint.z);
				D3DXMatrixRotationAxis(&rota, &direction, 0.0f);
				cur = mx * rota * tran;
				if (!_renderSystem->setWorldMatrix(&cur))
				{
					return false;  
				}

				//
				unsigned short unicode = 0;
				FTex* fTex = 0;
				_baseX = 0;
				unsigned int spaceOffset = _fontSize * 0.5;
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
						_baseX += spaceOffset;
					}
					else
					{
						_renderImpl(fTex, color, direction);
					}

					//
					unicode = 0;
					fTex = 0;
				}

				//
				_renderSystem->setWorldMatrix(&mx);
			}
			break;

			//
		default:
			break;
		}

		//
		return true;
	}

	bool FreeType::render(FontDimention fd, D3DXVECTOR3& basePoint, D3DXVECTOR3& direction, const D3DCOLOR& color, const char* text )
	{
		if (text == 0)
		{
			return true;
		}

		return render(fd, basePoint, direction, color, std::string(text) );
	}

	void FreeType::_renderImpl( FTex* fft, const D3DCOLOR& color, D3DXVECTOR3& direction )
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
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	


		//
		static unsigned short i[6] = {0, 1, 3, 1, 2, 3};

	
		// Fuck You!
		_renderSystem->setVertexDeclaration(VD_POSITION_COLOR_TEXTURE);

		//
		POSITION_COLOR_TEXTURE v[] = 
		{
			//
			POSITION_COLOR_TEXTURE(Vec3(_baseX + fft->_bearingX,					fft->_bearingY,					0.0f),
			color, 10 * fft->_uv0),

			POSITION_COLOR_TEXTURE(Vec3(_baseX + fft->_bearingX + fft->_width,	fft->_bearingY,					0),
			color, 10 * Vec2(fft->_uv2.x, fft->_uv0.y)),

			POSITION_COLOR_TEXTURE(Vec3(_baseX + fft->_bearingX + fft->_width,	-fft->_height + fft->_bearingY, 0),
			color, 10 * fft->_uv2),

			POSITION_COLOR_TEXTURE(Vec3(_baseX + fft->_bearingX,					-fft->_height + fft->_bearingY, 0),
			color, 10 * Vec2(fft->_uv0.x, fft->_uv2.y)),
		};

		//
		_renderSystem->drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, i, D3DFMT_INDEX16, v, sizeof(POSITION_COLOR_TEXTURE));

		//
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		_renderSystem->setVertexDeclaration(VD_NULL);

		//
		_baseX += fft->_advance;
	}


	void FreeType::_renderImpl2D( FTex* fft, const D3DCOLOR& color, D3DXVECTOR3& basePoint )
	{
		//
		if (!_renderSystem->setTexture(0, fft->_tex))
		{
			return;
		}
		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
		_renderSystem->setRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
		_renderSystem->setRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);

		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);	


		//
		static unsigned short i[6] = {0, 1, 2, 0, 2, 3};

		//
		// Fuck You!
		_renderSystem->setVertexDeclaration(VD_TPOSITION_COLOR_TEXTURE);

		float offsetY = 0.0f;
		offsetY = fft->_height;// - fft->_bearingY;
		//
		static const Real scScaleTexCoord = 1.0f;
		TPOSITION_COLOR_TEXTURE v[] = 
		{
			//
			TPOSITION_COLOR_TEXTURE(D3DXVECTOR4(basePoint.x + fft->_bearingX,					basePoint.y - fft->_bearingY,			0.5, 1.0f),
			color, scScaleTexCoord * D3DXVECTOR2(fft->_uv0.x, fft->_uv0.y)),

			TPOSITION_COLOR_TEXTURE(D3DXVECTOR4(basePoint.x + fft->_bearingX + fft->_width,		basePoint.y - fft->_bearingY,			0.5, 1.0f),
			color, scScaleTexCoord * D3DXVECTOR2(fft->_uv2.x, fft->_uv0.y)),

			TPOSITION_COLOR_TEXTURE(D3DXVECTOR4(basePoint.x + fft->_bearingX + fft->_width,		basePoint.y + offsetY,	0.5, 1.0f),
			color, scScaleTexCoord * D3DXVECTOR2(fft->_uv2.x, fft->_uv2.y)),

			TPOSITION_COLOR_TEXTURE(D3DXVECTOR4(basePoint.x + fft->_bearingX,					basePoint.y + offsetY,	0.5, 1.0f),
			color, scScaleTexCoord * D3DXVECTOR2(fft->_uv0.x, fft->_uv2.y)),
		};

		//
		_renderSystem->drawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, i, D3DFMT_INDEX16, v, sizeof(TPOSITION_COLOR_TEXTURE));

		//
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		_renderSystem->setVertexDeclaration(VD_NULL);

		//
		basePoint.x += fft->_advance;
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
		case FONT_PROP_BG_1:
			fft->_height += 2;
			fft->_width += 2;
			buffer = new unsigned int[fft->_width * fft->_height];
			memset(buffer, 0, sizeof(unsigned int) * fft->_width * fft->_height);
			switch (bitmap.pixel_mode)
			{
			case FT_PIXEL_MODE_MONO:
				{
					//
					unsigned char* buffer2 = new unsigned char[bitmap.width * bitmap.rows];
					memset(buffer2, 0, bitmap.width * bitmap.rows);
					for (int i = 0; i < bitmap.rows; ++i)
					{
						unsigned char *src = bitmap.buffer + (i * bitmap.pitch);
						unsigned int pitch = i * bitmap.rows;
						int j;
						for (j = 0; j < bitmap.width; ++j)
							buffer2[j + pitch] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFF : 0x00;
					}
					//
					for(int m = -1; m <= 1; ++m)
						for(int n = -1; n <= 1; ++n)
						{
							for (int i = 0; i < bitmap.rows; ++i)
							{
								for (int j = 0; j < bitmap.width; ++j)
								{
									unsigned char c = buffer2[i * bitmap.width + j];
									if (c > 0)
									{
										unsigned int e = buffer[(i+m+1) * (int)fft->_width + (j+n+1)];
										unsigned char v = e >> 24;
										if(c + v + alphaAddon > 255)
										{
											v = 255;
										}
										else
										{
											v += c;
											v += alphaAddon;
										}
										buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = (v << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
									}
								}
							}
						}
						if (buffer2)
						{
							delete buffer2;
							buffer2 = 0;
						}
				}
				break;
			case FT_PIXEL_MODE_GRAY:
				{
					//
					for(int m = -1; m <= 1; ++m)
						for(int n = -1; n <= 1; ++n)
						{
							for (int i = 0; i < bitmap.rows; ++i)
							{
								for (int j = 0; j < bitmap.width; ++j)
								{
									unsigned char c = bitmap.buffer[i * bitmap.pitch + j];
									if (c > 0)
									{
										unsigned int e = buffer[(i+m+1) * (int)fft->_width + (j+n+1)];
										unsigned char v = e >> 24;
										if(c + v + alphaAddon > 255)
										{
											v = 255;
										}
										else
										{
											v += c;
											v += alphaAddon;
										}
										buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = (v << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
									}
								}
							}
						}
				}
				break;
			}
			break;
		case FONT_PROP_PROJECT_1:
			{
				//fft->_advance += 1;
				fft->_bearingX += 1;
				fft->_bearingY -= 1;
				fft->_height += 2;
				fft->_width += 2;
				buffer = new unsigned int[fft->_width * fft->_height];
				memset(buffer, 0, sizeof(unsigned int) * fft->_width * fft->_height);
				//
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
							{
								buffer[j + pitch] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
								buffer[j + 1 + pitch + bitmap.rows] = (0xFF << 24) | (0x00<< 16) | (0x00 << 8) | 0x00;
							}
						}
					}
					break;
				case FT_PIXEL_MODE_GRAY:
					{
						//
						for (int i = 0; i < bitmap.rows; ++i)
						{
							for (int j = 0; j < bitmap.width; ++j)
							{
								unsigned char c = bitmap.buffer[i * bitmap.pitch + j];
								if(c > 0)
								{
									buffer[i * bitmap.width + j] = (c << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
									buffer[(i + 1) * bitmap.width + j + 1] = (0xFF << 24) | (0x00<< 16) | (0x00 << 8) | 0x00;
								}
							}
						}
					}
					break;
				}
			}
			break;
		case FONT_PROP_PROJECT_2:
			{
				//fft->_advance += 1;
				fft->_bearingX += 1;
				fft->_bearingY -= 1;
				fft->_height += 2;
				fft->_width += 2;
				buffer = new unsigned int[fft->_width * fft->_height];
				memset(buffer, 0, sizeof(unsigned int) * fft->_width * fft->_height);
				//
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
							{
								buffer[j + pitch] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFFFF : 0x00000000;
								buffer[j + 2 + pitch + 2*bitmap.rows] = (0xFF << 24) | (0x00<< 16) | (0x00 << 8) | 0x00;
							}
						}
					}
					break;
				case FT_PIXEL_MODE_GRAY:
					{
						//
						for (int i = 0; i < bitmap.rows; ++i)
						{
							for (int j = 0; j < bitmap.width; ++j)
							{
								unsigned char c = bitmap.buffer[i * bitmap.pitch + j];
								if(c > 0)
								{
									buffer[i * bitmap.width + j] = (c << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
									buffer[(i + 2) * bitmap.width + j + 2] = (0xFF << 24) | (0x00<< 16) | (0x00 << 8) | 0x00;
								}
							}
						}
					}
					break;
				}
			}
			break;
		case FONT_PROP_OUTLINE:
			{
				//fft->_advance += 1;
				fft->_bearingX -= 1;
				fft->_bearingY -= 1;
				fft->_height += 2;
				fft->_width += 2;
				buffer = new unsigned int[fft->_width * fft->_height];
				memset(buffer, 0, sizeof(unsigned int) * fft->_width * fft->_height);
				switch (bitmap.pixel_mode)
				{
				case FT_PIXEL_MODE_MONO:
					{
						//
						unsigned char* buffer2 = new unsigned char[bitmap.width * bitmap.rows];
						memset(buffer2, 0, bitmap.width * bitmap.rows);
						for (int i = 0; i < bitmap.rows; ++i)
						{
							unsigned char *src = bitmap.buffer + (i * bitmap.pitch);
							unsigned int pitch = i * bitmap.rows;
							int j;
							for (j = 0; j < bitmap.width; ++j)
								buffer2[j + pitch] = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFF : 0x00;
						}
						//
						for(int m = -1; m <= 1; ++m)
							for(int n = -1; n <= 1; ++n)
							{
								for (int i = 0; i < bitmap.rows; ++i)
								{
									for (int j = 0; j < bitmap.width; ++j)
									{
										unsigned char c = buffer2[i * bitmap.width + j];
										if (c > 0)
										{
											if (m == 0 && n == 0)
											{
												buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = (c << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
											}
											else
											{
												unsigned int e = buffer[(i+m+1) * (int)fft->_width + (j+n+1)];
												if (e == 0)
												{
													buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = 0xFF000000;
												}
											}
										}
									}
								}
							}
							if (buffer2)
							{
								delete buffer2;
								buffer2 = 0;
							}
					}
					break;
				case FT_PIXEL_MODE_GRAY:
					{
						//
						for(int m = -1; m <= 1; ++m)
							for(int n = -1; n <= 1; ++n)
							{
								for (int i = 0; i < bitmap.rows; ++i)
								{
									for (int j = 0; j < bitmap.width; ++j)
									{
										unsigned char c = bitmap.buffer[i * bitmap.pitch + j];
										if (c > 0)
										{
											if (m == 0 && n == 0)
											{
												buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = (c << 24) | (0xFF<< 16) | (0xFF << 8) | 0xFF;
											}
											else
											{
												unsigned int e = buffer[(i+m+1) * (int)fft->_width + (j+n+1)];
												if (e == 0)
												{
													buffer[(i+m+1) * (int)fft->_width + (j+n+1)] = 0xFF000000;
												}
											}
										}
									}
								}
							}
					}
				}
			}
			break;
			//
		case FONT_PROP_NONE:
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
			_activeTex = TextureFactory::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, D3DFMT_A8R8G8B8);
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
				_activeTex = TextureFactory::getInstancePtr()->createEmptyTexture(_TEXTURE_SIZE, _TEXTURE_SIZE, D3DFMT_A8R8G8B8);
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
