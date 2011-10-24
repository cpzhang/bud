//==========================================================================
/**
* @file	  : LightningStrip.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-7   10:05
* purpose : 
*/
//==========================================================================

#ifndef __LightningStrip_h__
#define __LightningStrip_h__

#include "Common.h"
#include "RenderableObject.h"
//
namespace Euclid
{
	class LightningStrip : public RenderableObject
	{
	public:
		LightningStrip();
		~LightningStrip();

		//
	public:
		//
		virtual bool create();
		//
		virtual void destroy();

		//
		virtual void update(u32 current, u32 delta);

		//
		virtual void render();

		//
		virtual void preRender();

		//
		virtual void postRender();

		//
	private:

		//
	private:
		IBufferFactory*			_bufferManager;
		VertexBuffer*			_vb;
		IndexBuffer*			_ib;
		ITexture*				_tex;
		Vec3			_begin;
		Vec3			_end;
		float					_cell_width;
		float					_cell_num;
		int						_vertices_num;
	};
}

#endif // __LightningStrip_h__
 
