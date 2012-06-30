/********************************************************************
**	file: 		IRenderSystem.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __IRenderSystem_h__
#define __IRenderSystem_h__

#include "Common.h"
#include "Color.h"
#include "RenderEngineCreationParameters.h"
namespace Euclid
{
	struct IRenderSystem
	{
		virtual bool create() = 0;
		virtual bool destroy() = 0;
		//
		virtual bool clear(
			u32 Count, 
			const Rect* pRects, 
			u32 Flags,
			Color Color,
			float Z,
			u32 Stencil
			) = 0;
		//
		virtual bool beginScene() = 0;
		virtual bool endScene() = 0;
		//
		virtual bool present(
			const Rect *pSourceRect,
			const Rect *pDestRect,
			HWND hDestWindowOverride
			) = 0;
		//
		virtual bool setVertexDeclaration(eVertexDeclarationType e) = 0;
		//
		virtual bool setStreamSource(
			u32 StreamNumber,
			IBuffer *pStreamData,
			u32 OffsetInBytes,
			u32 Stride
			) = 0;
		//
		virtual bool drawPrimitive(
			ePrimitive PrimitiveType,
			u32 StartVertex,
			u32 PrimitiveCount
			) = 0;
		//
		virtual bool drawPrimitiveUP(
			ePrimitive PrimitiveType,
			u32 PrimitiveCount,
			const void *pVertexStreamZeroData,
			u32 VertexStreamZeroStride
			) = 0;

		//
		virtual bool drawIndexedPrimitive(
			ePrimitive Type,
			s32 BaseVertexIndex,
			u32 MinIndex,
			u32 NumVertices,
			u32 StartIndex,
			u32 PrimitiveCount
			) = 0;
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
			) = 0;

		//
		virtual bool setTexture(
			u32 Sampler,
			ITexture* pTexture
			) = 0;
		virtual bool setStreamSourceFreq(
			u32 StreamNumber,
			u32 FrequencyParameter
			) = 0;
		virtual bool setIndices(
			IBuffer *pIndexData
			) = 0;
		virtual bool setRenderState(
			eRenderState State,
			u32 Value
			) = 0;
		virtual bool getRenderState(
			eRenderState State,
			u32 *pValue
			) = 0;
		virtual bool setViewport(
			const sViewPort *pViewport
			) = 0;
		virtual void getViewPort(sViewPort* vp) = 0;
		// The default render target is called the back buffer
		// A render target has a width and height. The width and height of the back buffer are the final resolution of your game.
		// An offscreen render target does not need to have the same width and height as the back buffer. 
		// Small parts of the final image can be rendered in small render targets, then copied to another render target later.
		// A render target also has a surface format, which describes how many bits are allocated to each pixel and how they are divided between red, green, blue, and alpha.
		//
		virtual bool setRenderTarget(
			u32 RenderTargetIndex,
			ITexture* tex
			//IDirect3DSurface9 *pRenderTarget
			) = 0;

		//
		virtual Vec3* objectSpace2ScreenSpace(
			__inout  Vec3 *pOut,
			__in     const Vec3 *pV,
			__in     const sViewPort *pViewport,
			__in     const Mat4 *pProjection,
			__in     const Mat4 *pView,
			__in     const Mat4 *pWorld
			) = 0;

	};
}

#endif // __IRenderSystem_h__