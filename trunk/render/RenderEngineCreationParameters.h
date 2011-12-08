/********************************************************************
**	file: 		RenderEngineCreationParameters.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-8
**	purpose:	
*********************************************************************/
#ifndef __RenderEngineCreationParameters_h__
#define __RenderEngineCreationParameters_h__
#include "base/Buddha.h"
#include "external/directx/Include/d3d9.h"
namespace Euclid
{
	struct RenderEngineCreationParameters : public Buddha::SingletonEx<RenderEngineCreationParameters>
	{
		RenderEngineCreationParameters()
		{
			hFocusWindow = 0;
		}
		HWND hFocusWindow;
	};

	enum eClearFlags
	{
		eClearFlags_Target = D3DCLEAR_TARGET,
		eClearFlags_ZBuffer = D3DCLEAR_ZBUFFER,
		eClearFlags_Stencil = D3DCLEAR_STENCIL,
	};
}

#endif // __RenderEngineCreationParameters_h__
