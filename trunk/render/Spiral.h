//==========================================================================
/**
* @file	  : Spiral.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-7   14:45
* purpose : 
*/
//==========================================================================

#ifndef __Spiral_h__
#define __Spiral_h__

#include "Common.h"
#include "RenderableObject.h"
//
namespace Euclid
{
	class Spiral : public RenderableObject
	{
	public:
		Spiral();
		~Spiral();

		//
	public:
		//
		virtual bool create();
		//
		virtual void destroy();

		//
		virtual void update();

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

#endif // __Spiral_h__
 
