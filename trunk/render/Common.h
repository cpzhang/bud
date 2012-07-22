//==========================================================================
/**
* @file	  : Common.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-24   21:15
* purpose : 
*/
//==========================================================================

#ifndef __RenderCommon_h__
#define __RenderCommon_h__

//
#include "base/Buddha.h"
#include "math/Euler.h"

//
//#define D3D_DEBUG_INFO	

#include "external/directx/Include/d3d9.h"
#include "external/directx/Include/d3dx9.h"
//DXerr跟vs兼容性有问题
//#include "external/directx/Include/DxErr.h"
#ifndef _DXERR_H_
	#define DXGetErrorString
	#define DXGetErrorDescription
#endif
#include "RenderEngineCreationParameters.h"
//
namespace Zen
{
	//
#if defined(render_EXPORTS) // inside DLL
#   define _ZenExport_   __declspec(dllexport)
#else // outside DLL
#   define _ZenExport_   __declspec(dllimport)
#endif

	// declearation classes
	struct IRenderSystem;
	struct Rect;
	class IMaterial;
	class IMaterialManager;
	class IBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class BufferManager;
	class Effect;
	class EffectManager;
	class Camera;
	class CameraControllerThirdPerson;
	class ITexture;
	class Texture;
	class ITextureManager;
	class TextureManager;
	class FontManager;
	class FreeType;
	class Terrain;
	class MZ;
	class Mesh;
}
const int MODEL_NAME_NODE_SIZE             = 80;

//
#endif // __RenderCommon_h__
