//==========================================================================
/**
* @file	  : EntityXX.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   19:52
* purpose : 
*/
//==========================================================================

#ifndef __EntityXX_h__
#define __EntityXX_h__

#include "stdafx.h"
#include "Common.h"
#include "RenderableObject.h"
//
namespace Euclid
{
	class _EuclidExport_ EntityX : public RenderableObject
	{
	public:
		EntityX();
		~EntityX();

		//
	public:
		virtual bool create();
		virtual void destroy();

		virtual void preRender();
		virtual void postRender();
		//
		virtual void renderNormal();
		virtual void renderWithEffect();

	public:
		bool loadMeshFromX(const std::string& file, unsigned long flag = D3DXMESH_SYSTEMMEM);

		//bool loadEffectFileFromFile(const std::string& file);

		bool getBoundingSphere(Sphere& s);

		bool getBoundingAABB(AABB& s);

		//
	private:
		LPD3DXMESH				_mesh;
		IDirect3DDevice9*		_device;
		ID3DXBuffer*			_xMaterialBuffer;
		unsigned long			_numMaterial;
		D3DMATERIAL9*			_materials;
		//LPDIRECT3DTEXTURE9*		_textures;
		std::vector<ITexture*>	_textures;
		Sphere*					_boundingSphere;
		AABB*					_boundingAABB;
		//ID3DXEffect*			_effect;
		//ITexture*				_texEffect;
	};
}

#endif // __EntityXX_h__
 
