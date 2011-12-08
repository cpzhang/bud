//==========================================================================
/**
* @file	  : VertexDeclaration.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   9:46
* purpose : 
*/
//==========================================================================

#ifndef __VertexDeclaration_h__
#define __VertexDeclaration_h__

#include "stdafx.h"
#include "Common.h"
#include "Color.h"

namespace Euclid
{
	enum eFormat_Index
	{
		eFormat_Index_16,
		eFormat_Index_32,
		eFormat_Index_Max
	};

	enum ePrimitiveType
	{
		ePrimitiveType_PointList,
		ePrimitiveType_LineList,
		ePrimitiveType_LineStrip,
		ePrimitiveType_TriangleList,
		ePrimitiveType_TriangleStrip,
		ePrimitiveType_TriangleFan,
		ePrimitiveType_Max
	};

	//
	enum eVertexDeclarationType
	{
		//
		VD_NULL,
		//
		VD_POSITION,
		//
		VD_POSITION_COLOR,
		//
		VD_TPOSITION_COLOR,
		//
		VD_POSITION_NORMAL,
		//
		VD_POSITION_TEXTURE,
		//
		VD_TPOSITION_TEXTURE,
		//
		VD_POSITION_COLOR_TEXTURE,
		//
		VD_TPOSITION_COLOR_TEXTURE,
		//
		VD_POSITION_NORMAL_TEXTURE,
		//
		VD_COUNT
	};

// #pragma pack( push)
// #pragma pack( 1 )
	
	//
	struct sPOSITION
	{
		sPOSITION()
		{

		}

		sPOSITION(Vec3 pos)
			: _pos(pos)
		{
		}

		//
		sPOSITION(float x, float y, float z)
		{
			_pos.x = x;
			_pos.y = y;
			_pos.z = z;
		}

		//
		Vec3	_pos;
	};

	//
	struct POSITION_COLOR
	{
		POSITION_COLOR()
		{

		}

		POSITION_COLOR(Vec3 pos, RGBA color)
			: _pos(pos), _color(color)
		{
		}

		//
		POSITION_COLOR(float x, float y, float z, unsigned long color)
		{
			_pos.x = x;
			_pos.y = y;
			_pos.z = z;
			//
			_color = color;
		}

		//
		Vec3	_pos;
		RGBA	_color;
	};

	//
	struct TPOSITION_COLOR
	{
		TPOSITION_COLOR(D3DXVECTOR4 pos, D3DCOLOR color)
			: _pos(pos), _color(color)
		{
		}

		//
		TPOSITION_COLOR(float x, float y, float z, float w, unsigned long color)
		{
			_pos.x = x;
			_pos.y = y;
			_pos.z = z;
			_pos.w = w;
			_color = color;
		}

		D3DXVECTOR4 _pos;
		D3DCOLOR	_color;
	};

	//
	struct POSITION_NORMAL
	{
		POSITION_NORMAL()
		{

		}

		POSITION_NORMAL(const Vec3& pos, const Vec3& normal)
			: _pos(pos), _normal(normal)
		{
		}
	
		Vec3 _pos;
		Vec3 _normal;
	};

	//
	struct POSITION_TEXTURE
	{
		POSITION_TEXTURE(const Vec3& pos, Vec2 tex)
			: _pos(pos), _tex(tex)
		{
		}

		POSITION_TEXTURE(float x, float y, float z, float u, float v)
			: _pos(Vec3(x, y, z)), _tex(Vec2(u, v))
		{
		}

		Vec3 _pos;
		Vec2	_tex;
	};

	//
	struct TPOSITION_TEXTURE
	{
		inline TPOSITION_TEXTURE& operator = (const TPOSITION_TEXTURE& r)
		{
			_pos = r._pos;
			_tex = r._tex;

			return *this;
		}
		TPOSITION_TEXTURE()
		{

		}

		TPOSITION_TEXTURE(float x, float y, float z, float w, float u, float v)
			:	_pos(Vec4(x, y, z, w)), _tex(Vec2(u, v))  
		{

		}
		TPOSITION_TEXTURE(Vec4 pos, Vec2 tex)
			: _pos(pos), _tex(tex)
		{
		}
		Vec4	_pos;
		Vec2	_tex;
	};

	//
	struct POSITION_COLOR_TEXTURE
	{
		POSITION_COLOR_TEXTURE()
		{

		}
		POSITION_COLOR_TEXTURE(Vec3 pos, RGBA color, Vec2 tex)
			: _pos(pos), _color(color), _tex(tex)
		{
		}
		Vec3 _pos;
		RGBA _color;
		Vec2 _tex;
	};

	//
	struct TPOSITION_COLOR_TEXTURE
	{
		TPOSITION_COLOR_TEXTURE(D3DXVECTOR4 pos, D3DCOLOR color, D3DXVECTOR2 tex)
			: _pos(pos), _color(color), _tex(tex)
		{
		}
		D3DXVECTOR4 _pos;
		D3DCOLOR	_color;
		D3DXVECTOR2	_tex;
	};

	//
	//
	struct POSITION_NORMAL_TEXTURE
	{
		POSITION_NORMAL_TEXTURE()
		{
			memset(&_pos, 0, sizeof(_pos));
			memset(&_normal, 0, sizeof(_normal));
			memset(&_tex, 0, sizeof(_tex));
		}
		POSITION_NORMAL_TEXTURE(D3DXVECTOR3 pos, D3DXVECTOR3 normal, D3DXVECTOR2 tex)
			: _pos(pos), _normal(normal), _tex(tex)
		{
		}
		D3DXVECTOR3 _pos;
		D3DXVECTOR3	_normal;
		D3DXVECTOR2	_tex;
	};

	//#pragma pack( pop )
}

#endif // __VertexDeclaration_h__
