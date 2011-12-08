//==========================================================================
/**
* @file	  : Billboard.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-17   11:15
* purpose : 
*/
//==========================================================================

#ifndef __Billboard_h__
#define __Billboard_h__

#include "Common.h"
#include "RenderableObject.h"
#include "Color.h"

namespace Euclid
{
	class _EuclidExport_ BillboardPara : public RenderablePara
	{
	public:
		BillboardPara(); 
		BillboardPara(BillboardPara* bp);
		~BillboardPara();
	public:
		Vec3		_center;
		Vec3		_up;
		Real		_offset;
		std::string _texName;
		u32			_color;
		bool		_update;
	};
	class Billboard :  public RenderableObject
	{
	public:
		Billboard(BillboardPara* para);
		~Billboard();

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

	public:
		void setCenter(Vec3 center);
		Vec3 getCenter() const;
		void setOffset(Real offset);
		Real getOffset() const;
		void setColor(RGBA color);
	private:
		Vec3		_center;
		Vec3		_normal;
		Vec3		_up;
		Real		_offset;
		Camera*		_camera;
		ITexture*   _tex;
		VertexBuffer*	_vb;
		IndexBuffer*	_ib;
		BillboardPara*	_para;
		static const int _VertexCount;
	};
}

#endif // __Billboard_h__
 
