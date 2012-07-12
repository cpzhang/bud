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
		RenderSystem_Derived
	public:
		RenderSystem();
		~RenderSystem();

		IDirect3DDevice9* getDevice();
	private:
		void initVertexDeclarations();
	private:
		IDirect3D9*	_d3d9;
		IDirect3DDevice9* _device;
		D3DCAPS9				_caps;
		//
		IDirect3DVertexDeclaration9*	_vertexDeclarations[eVertexDeclarationType_Size];
		//
		D3DPRESENT_PARAMETERS _presentationParameters;
		static 	LPDIRECT3DSURFACE9 s_pRenderSurface;
	};
}

#endif // __RenderSystem_h__