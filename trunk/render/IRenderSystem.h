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

	};
}

#endif // __IRenderSystem_h__