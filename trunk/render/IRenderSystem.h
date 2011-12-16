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
	};
}

#endif // __IRenderSystem_h__