//==========================================================================
/**
* @file	  : Blooding.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-8   17:18
* purpose : 
*/
//==========================================================================

#ifndef __Blooding_h__
#define __Blooding_h__

#include "Common.h"
#include "RenderableObject.h"
//
namespace Euclid
{
	class ManualPara : public RenderablePara
	{
	public:
	public:
		char					_type[64];
		char					_name[64];
		Real					_height;
		Real					_heightEx;
		char					_texFile[64];
		bool					_rotate;
		Real					_rotateSpeed;
		
		bool					_updateUV;
		Real					_uvSpeed;

		bool					_updateVertex;
		Real					_vertexSpeed;
	};

	class Blooding : public RenderableObject
	{
	public:
		Blooding(std::string texFile, Real speed);
		Blooding();
		Blooding(ManualPara* para);
		~Blooding();

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
		bool updateUV();
		bool updateVertex();
		bool rotation_pitch();
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
		Vec2*	_u;
		std::string				_texFile;
		Real					_rotateSpeed;
		Real					_j;
		int						_lastJ;
		Real					_height;
		Real					_heightEx;
		bool					_updateUV;
		bool					_updateVertex;
		Real					_indexU;
		ManualPara*				_para;
		float					_flowedU;
	};
}

#endif // __Blooding_h__
