//==========================================================================
/**
* @file	  : IRenderSystem.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   21:48
* purpose : 
*/
//==========================================================================

#ifndef __IRenderSystem_h__
#define __IRenderSystem_h__

#include "Common.h"
#include "VertexDeclaration.h"
//#include "ITexture.h"
//#include "VertexBuffer.h"
//#include "IndexBuffer.h"
//#include "Material.h"
//#include "Light.h"
//
namespace Euclid
{
	class IRenderSystem
	{
	public:
		virtual bool				update(u32 current, u32 delta) = 0;
		virtual bool				create() = 0;
		virtual void				destroy() = 0;
		
		//
		virtual bool                setViewport(const D3DVIEWPORT9 * pViewport) = 0;
		virtual bool                getViewport(D3DVIEWPORT9 * pViewport) = 0;

		virtual bool				getWorldMatrix(D3DMATRIX * pMatrix) = 0;
		virtual bool				setWorldMatrix(const D3DMATRIX * pMatrix) = 0;
		virtual bool				setWorldMatrix(const  Mat4& pM) = 0;


		virtual bool				setViewMatrix(const D3DMATRIX * pMatrix) = 0;
		virtual bool				setViewMatrix(const  Mat4& pM) = 0;
		virtual bool				setProjectionMatrix(const  Mat4& pM) = 0;
		virtual bool				setProjectionMatrix(const D3DMATRIX * pMatrix) = 0;
		virtual bool				setTexture(unsigned short index, ITexture* tex)	= 0;
		virtual bool				setVertexDeclaration(eVertexDeclarationType vd) = 0;
		virtual bool				setTextureStageState(unsigned long Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned long Value) = 0;
		virtual bool				setRenderState(D3DRENDERSTATETYPE State, unsigned long Value) = 0;
		virtual bool				setSamplerState(unsigned long Sampler, D3DSAMPLERSTATETYPE Type, unsigned long Value) = 0;
		virtual bool				setLight(unsigned int index, Light* light) = 0;
		virtual bool				setMaterial(Material* material) = 0;

		virtual bool				setTextureMatrix(u32 index, const Mat4& m) = 0;
		//
		virtual bool				setStreamSource(unsigned int StreamNumber, VertexBuffer* pStreamData, unsigned int OffsetInBytes, unsigned int Stride) = 0;
		virtual bool				setIndices(IndexBuffer* pIndexData) = 0;



		//
		virtual bool				drawPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex,	unsigned int PrimitiveCount) = 0;
		virtual bool				drawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) = 0;
		virtual bool				drawIndexedPrimitive(D3DPRIMITIVETYPE Type,	int BaseVertexIndex, unsigned int MinIndex, unsigned int NumVertices, unsigned int StartIndex, unsigned int PrimitiveCount) = 0;
		virtual bool				drawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int PrimitiveCount, const void * pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) = 0;
	

		//
		virtual void				beginFrame(bool clearColorBuffer = true, bool clearDepthBuffer = true, bool clearStencilBuffer = false) = 0;
		virtual void				endFrame() = 0;

		//
		virtual bool				queryShaderCaps(unsigned int major, unsigned int minor) = 0;

		//
		virtual void				drawPoint(Vec3 point, RGBA color) = 0;
		virtual void				drawLine(Vec3 beginPoint, Vec3 endPoint, RGBA color) = 0;

		//
		//virtual void				switchProjectionMode() = 0;
		//virtual bool				isPerspectiveProjectionMode() = 0;
		//virtual bool				isOrthographicProjectionMode() = 0;

		//
		virtual void				resetCamera() = 0;

		virtual void				resetDevice() = 0;

		virtual void				drawQuad(const POSITION_TEXTURE* v, ITexture* tex) = 0;

		virtual void				drawQuad(const TPOSITION_TEXTURE* v, ITexture* tex) = 0;

		//virtual bool				initCEGUIRenderer(CEGUI::Direct3D9Renderer& renderer) = 0;
		virtual Ray					getPickingRay(Vec2& point, Mat4& mProj) = 0;

		virtual LPDIRECT3DDEVICE9	getDevice() = 0;

		//
		virtual void onLostDevice() = NULL;

		//
		virtual void onResetDevice() = NULL;

		//
		virtual void setVertexShader(IShader* s) = NULL;

		//
		virtual void setPixelShader(IShader* s) = NULL;

		//
		virtual void setClearColor(Color& c) = NULL;
	};
}

#endif // __IRenderSystem_h__
 
