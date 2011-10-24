//==========================================================================
/**
* @file	  : ManualObject.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-8   11:43
* purpose : 
*/
//==========================================================================

#ifndef __ManualObject_h__
#define __ManualObject_h__

#include "Common.h"
#include "RenderableObject.h"
#include "VertexDeclaration.h"

//
namespace Euclid
{
	class _EuclidExport_ ManualObject : public RenderableObject
	{
	public:
		ManualObject(const std::string& name);
		~ManualObject();

		//
	public:
		//
		virtual bool create();
		
		//
		virtual void destroy();

		//
		virtual void update(u32 current, u32 delta);

		//
		virtual void renderNormal();
		virtual void renderWithEffect();

		//
		virtual void preRender();

		//
		virtual void postRender();

	public:
		//
		bool setVertexDeclarationType(eVertexDeclarationType t);

		//
		bool setUnitSize(size_t i);

		//
		bool setPrimitiveType(ePrimitiveType t);
		//
		bool setPrimitiveCount(size_t ptCount);

		//
		bool setMaterial(Material* m);

		//
		bool setLight(Light* l);

		//
		void addVertex(const Vec3& p);

		//
		void addVertex(const Vec3& p, RGBA c);
	
		//
		void addVertex(const Vec3& p, const Vec2& t);

		//
		void addVertex(const Vec3& p, const Vec3& n);

		//
		void addVertex(const Vec3& p, RGBA c, const Vec2& t);

		//
		void setIndices(u16* d, u32 n);

		void setTexture(u16 index, ITexture* t);

		ITexture* getTexture(u16 index);

		void setWireFrame(bool wf);
		void setSelected(bool b);
		//
	private:
		size_t _getPrimitiveCount();
		
		//
	private:
		IBufferFactory*			_bufferManager;
		VertexBuffer*			_vb;
		IndexBuffer*			_ib;
		std::vector<ITexture*>				_texs;
		Vec3			_begin;
		Vec3			_end;
		float					_cell_width;
		float					_cell_num;
		int						_vertices_num;
		u16*					_indices16;
		u32						_indicesCount16;
		u16*					_indices32;
		u32						_indicesCount32;
		//
		eVertexDeclarationType	_vdt;
		std::vector<u32>	_vertices[VD_COUNT];
		size_t				_unitSize;
		ePrimitiveType		_pt;
		Material*			_material;
		Light*				_light;
		bool				_wireFrame;
		bool				_isSelected;
		size_t				_primitiveCount;
	};
}

#endif // __ManualObject_h__
