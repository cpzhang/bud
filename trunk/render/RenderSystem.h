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
		virtual bool drawPrimitiveUP(
			ePrimitive PrimitiveType,
			u32 PrimitiveCount,
			const void *pVertexStreamZeroData,
			u32 VertexStreamZeroStride
			);
		//
		virtual bool drawIndexedPrimitive(
			ePrimitive Type,
			s32 BaseVertexIndex,
			u32 MinIndex,
			u32 NumVertices,
			u32 StartIndex,
			u32 PrimitiveCount
			);
		//
		virtual bool drawIndexedPrimitiveUP(
			ePrimitive PrimitiveType,
			u32 MinVertexIndex,
			u32 NumVertices,
			u32 PrimitiveCount,
			const void *pIndexData,
			eFormat IndexDataFormat,
			const void *pVertexStreamZeroData,
			u32 VertexStreamZeroStride
			);
		//
		virtual bool setTexture(
			u32 Sampler,
			ITexture* pTexture
			);
		//
		virtual bool setStreamSourceFreq(
			u32 StreamNumber,
			u32 FrequencyParameter
			);

		virtual bool setIndices(
			IBuffer *pIndexData
			);

		virtual bool setRenderState(
			eRenderState State,
			u32 Value
			);
		virtual bool getRenderState(
			eRenderState State,
			u32 *pValue
			);

		virtual bool setViewport(
			const sViewPort *pViewport
			);
		virtual void getViewPort(sViewPort* vp);
		virtual bool setRenderTarget(
			u32 RenderTargetIndex,
			ITexture* tex
			//IDirect3DSurface9 *pRenderTarget
			);
	public:
		RenderSystem();
		~RenderSystem();

		IDirect3DDevice9* getDevice();
	private:
		void initVertexDeclarations();
	private:
		IDirect3D9*	_d3d9;
		IDirect3DDevice9* _device;
		//
		IDirect3DVertexDeclaration9*	_vertexDeclarations[eVertexDeclarationType_Size];
		//
		D3DPRESENT_PARAMETERS _presentationParameters;
		static 	LPDIRECT3DSURFACE9 s_pRenderSurface;
	};
}

#endif // __RenderSystem_h__