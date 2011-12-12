/********************************************************************
**	file: 		RenderSystem.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __RenderSystem_h__
#define __RenderSystem_h__

#include "IRenderSystem.h"
#include "base/Buddha.h"
#include "external/directx/Include/d3d9.h"
#include "RenderEngineCreationParameters.h"
namespace Euclid
{
	class RenderSystem: public IRenderSystem, public Buddha::SingletonEx<RenderSystem>
	{
		// IRenderSystem
	public:
		virtual bool create();
		virtual bool destroy();

		bool clear(
			u32 Count, 
			const Rect* pRects, 
			u32 Flags,
			Color Color,
			float Z,
			u32 Stencil
			);

		virtual bool beginScene();
		virtual bool endScene();

		bool present(
			const Rect *pSourceRect,
			const Rect *pDestRect,
			HWND hDestWindowOverride
			);

		virtual bool setVertexDeclaration(eVertexDeclarationType e);

		virtual bool setStreamSource(
			u32 StreamNumber,
			IBuffer *pStreamData,
			u32 OffsetInBytes,
			u32 Stride
			);

		//
		virtual bool drawPrimitive(
			ePrimitive PrimitiveType,
			u32 StartVertex,
			u32 PrimitiveCount
			);
		//
		//
		virtual bool setTexture(
			u32 Sampler,
			ITexture* pTexture
			);

	public:
		RenderSystem();
		~RenderSystem();

		IDirect3DDevice9* getDevice();
		void getViewPort(D3DVIEWPORT9* vp);
	private:
		void initVertexDeclarations();
	private:
		IDirect3D9*	_d3d9;
		IDirect3DDevice9* _device;
		//
		IDirect3DVertexDeclaration9*	_vertexDeclarations[eVertexDeclarationType_Size];
	};
}

#endif // __RenderSystem_h__