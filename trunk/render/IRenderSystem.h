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
namespace Zen
{
	struct IRenderSystem
	{
		~IRenderSystem(){}
#define InterfaceRenderSystem(terminal)\
	public:\
		virtual bool create() ##terminal\
		virtual bool destroy() ##terminal\
		virtual bool clear(u32 Count, const Rect* pRects, u32 Flags, Color Color, float Z, u32 Stencil) ##terminal\
		virtual bool beginScene() ##terminal\
		virtual bool endScene() ##terminal\
		virtual bool present(const Rect *pSourceRect, const Rect *pDestRect, HWND hDestWindowOverride) ##terminal\
		virtual bool setVertexDeclaration(eVertexDeclarationType e) ##terminal\
		virtual bool setStreamSource(u32 StreamNumber, IBuffer *pStreamData, u32 OffsetInBytes, u32 Stride) ##terminal\
		virtual bool drawPrimitive(ePrimitive PrimitiveType, u32 StartVertex, u32 PrimitiveCount) ##terminal\
		virtual bool drawPrimitiveUP(ePrimitive PrimitiveType, u32 PrimitiveCount, const void *pVertexStreamZeroData, u32 VertexStreamZeroStride) ##terminal\
		virtual bool drawIndexedPrimitive(ePrimitive Type, s32 BaseVertexIndex,u32 MinIndex,u32 NumVertices,u32 StartIndex,u32 PrimitiveCount) ##terminal\
		virtual bool drawIndexedPrimitiveUP(ePrimitive PrimitiveType,u32 MinVertexIndex,u32 NumVertices,u32 PrimitiveCount, const void *pIndexData, eFormat IndexDataFormat, const void *pVertexStreamZeroData, u32 VertexStreamZeroStride ) ##terminal\
		virtual bool setTexture( u32 Sampler, ITexture* pTexture ) ##terminal\
		virtual bool setStreamSourceFreq( u32 StreamNumber, u32 FrequencyParameter ) ##terminal\
		virtual bool setIndices( IBuffer *pIndexData ) ##terminal\
		virtual bool setRenderState( eRenderState State, u32 Value ) ##terminal\
		virtual bool getRenderState( eRenderState State, u32 *pValue ) ##terminal\
		virtual bool setViewport( const sViewPort *pViewport ) ##terminal\
		virtual void getViewPort(sViewPort* vp) ##terminal\
		virtual bool setRenderTarget( u32 RenderTargetIndex, ITexture* tex) ##terminal\
		virtual Vec3* objectSpace2ScreenSpace( __inout  Vec3 *pOut, __in     const Vec3 *pV, __in     const sViewPort *pViewport, __in     const Mat4 *pProjection, __in     const Mat4 *pView, __in     const Mat4 *pWorld ) ##terminal\
		virtual DWORD getVertexShaderVersion() ##terminal\
		virtual DWORD getPixelShaderVersion() ##terminal\

#define RenderSystem_Base InterfaceRenderSystem(=0;)
#define RenderSystem_Derived InterfaceRenderSystem(;)
		RenderSystem_Base
	};
}

#endif // __IRenderSystem_h__
