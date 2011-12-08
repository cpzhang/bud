//==========================================================================
/**
* @file	  : RenderSystem.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   21:48
* purpose : 
*/
//==========================================================================

#ifndef __RenderSystem_h__
#define __RenderSystem_h__

#include "Common.h"
#include "IRenderSystem.h"
//
namespace Euclid
{
	class RenderSystem : public IRenderSystem
	{
	public:
		RenderSystem();
		~RenderSystem();

		//
	public:
		virtual bool	update(u32 current, u32 delta);
		virtual bool	create();
		virtual void	destroy();
		
		//
		virtual bool    setViewport(const D3DVIEWPORT9 * pViewport);
		virtual bool    getViewport(D3DVIEWPORT9 * pViewport);

		virtual bool	setWorldMatrix(const D3DMATRIX * pMatrix);
		virtual bool	getWorldMatrix(D3DMATRIX * pMatrix);
		virtual bool	setWorldMatrix(const  Mat4& pM);

		virtual bool	setViewMatrix(const D3DMATRIX * pMatrix);
		virtual bool	setViewMatrix(const  Mat4& pM);
		virtual bool	setProjectionMatrix(const  Mat4& pM);
		virtual bool	setProjectionMatrix(const D3DMATRIX * pMatrix);
		virtual bool	setTexture(unsigned short index, ITexture* tex);	
		virtual bool	setVertexDeclaration(eVertexDeclarationType vd);
		virtual bool	setTextureStageState(unsigned long Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned long Value);
		virtual bool	setRenderState(D3DRENDERSTATETYPE State, unsigned long Value);
		virtual bool	setSamplerState(unsigned long Sampler, D3DSAMPLERSTATETYPE Type, unsigned long Value);
		virtual bool	setLight(unsigned int index, Light* light);
		virtual bool	setMaterial(Material* material);

		virtual bool	setTextureMatrix(u32 index, const Mat4& m);

		virtual bool	setStreamSource(unsigned int StreamNumber, VertexBuffer* pStreamData, unsigned int OffsetInBytes, unsigned int Stride);
		virtual bool	setIndices(IndexBuffer* pIndexData);

		virtual void	beginFrame(bool clearColorBuffer = true, bool clearDepthBuffer = true, bool clearStencilBuffer = false);
		virtual void	endFrame();
		virtual bool	queryShaderCaps(unsigned int major, unsigned int minor);
		virtual void	drawPoint(Vec3 point, RGBA color);
		virtual void	drawLine(Vec3 beginPoint, Vec3 endPoint, RGBA color);
		virtual bool	drawPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex,	unsigned int PrimitiveCount);
		virtual bool    drawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride);
		virtual bool	drawIndexedPrimitive(D3DPRIMITIVETYPE Type,	int BaseVertexIndex, unsigned int MinIndex, unsigned int NumVertices, unsigned int StartIndex, unsigned int PrimitiveCount);
		virtual bool	drawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int PrimitiveCount, const void * pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride);

		//virtual void				switchProjectionMode();
		//virtual bool				isPerspectiveProjectionMode();
		//virtual bool				isOrthographicProjectionMode();

		virtual void				resetCamera();
		virtual void				resetDevice();

		virtual void	drawQuad(const POSITION_TEXTURE* v, ITexture* tex);

		virtual void				drawQuad(const TPOSITION_TEXTURE* v, ITexture* tex);

		//virtual bool				initCEGUIRenderer(CEGUI::Direct3D9Renderer& renderer);

		virtual Ray					getPickingRay(Vec2& point, Mat4& mProj);

		virtual LPDIRECT3DDEVICE9	getDevice();

		//
		virtual void onLostDevice();

		//
		virtual void onResetDevice();

		//
		virtual void setVertexShader(IShader* s);

		//
		virtual void setPixelShader(IShader* s);
		
		//
		virtual void setClearColor(Color& c);
		//
	private:
		//
		void _initPresentParas();
		
		//
		void _initVertexDeclaration();
		
		//
	private:
		//
		IDirect3D9*				_d3d9;

		//
		IDirect3DDevice9*		_device;

		//
		D3DCAPS9				_caps;
		
		//
		D3DPRESENT_PARAMETERS	_presentParas;

		//
		HWND					_hwnd;

		//
		IDirect3DVertexDeclaration9*	_vertexDeclaration[VD_COUNT];

		//
		Camera*					_camera;

		//
		bool					_3DMode;

		//
		Color					_clearColor;
	};
}

#endif // __RenderSystem_h__
 
