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
		//
	public:
		RenderSystem();
		~RenderSystem();
	private:
		IDirect3D9*	_d3d9;
		IDirect3DDevice9* _device;
	};
}

#endif // __RenderSystem_h__