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
#include "math/Euler.h"
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

	enum eVertexDeclarationType
	{
		eVertexDeclarationType_Null,
		eVertexDeclarationType_Position,
		eVertexDeclarationType_PositionColor,
		eVertexDeclarationType_PositionTexture,
		eVertexDeclarationType_PositionTextureNormal,
		eVertexDeclarationType_PositionColorTexture,
		eVertexDeclarationType_PositionTTexture,
		eVertexDeclarationType_Matrix,
		eVertexDeclarationType_PositionTextureNormalMatrix,
		eVertexDeclarationType_PositionTextureNormalIndex,
		eVertexDeclarationType_Size,
	};

	//
	struct sPosition
	{
		//
		Vec3	position;
	};

	//
	struct sPositionColor
	{
		//
		Vec3	position;
		//
		u32		color_ARGB;
	};
	//
	struct sPositionTexture
	{
		//
		Vec3	position;
		//
		Vec2	texcoord;
	};
	//
	struct sPositionTTexture
	{
		//
		Vec4	position;
		//
		Vec2	texcoord;
	};
	//
	struct sPositionColorTexture
	{
		//
		Vec3	position;
		//
		u32		color_ARGB;
		//
		Vec2	texcoord;
	};
	//
	struct sPositionTextureNormal
	{
		//
		Vec3	position;
		//
		Vec2	texcoord;
		//
		Vec3	normal;
	};
	//
	struct sPositionTextureNormalIndex
	{
		//
		Vec3	position;
		//
		Vec2	texcoord;
		//
		Vec3	normal;
		//
		float	index;
	};

	/* Pool types */
	enum ePool
	{
		ePool_Default = D3DPOOL_DEFAULT,// reset时，首先release，然后重新load
		ePool_Manager = D3DPOOL_MANAGED,// reset时，无需处理
		ePool_SystemMem = D3DPOOL_SYSTEMMEM,
		ePool_Scratch = D3DPOOL_SCRATCH,
		ePool_Size ,
	};

	// Usage
	enum eUsage
	{
		eUsage_Null = 0,
		eUsage_RenderTarget = D3DUSAGE_RENDERTARGET,
		eUsage_DepthStencil = D3DUSAGE_DEPTHSTENCIL,
		eUsage_Dynamic = D3DUSAGE_DYNAMIC,
		eUsage_AutoGenMipmap = D3DUSAGE_AUTOGENMIPMAP,
		eUsage_DMap = D3DUSAGE_DMAP,

		/* Usages for Vertex/Index buffers */
		eUsage_WriteOnly = D3DUSAGE_WRITEONLY,
		eUsage_SoftwareProcessing = D3DUSAGE_SOFTWAREPROCESSING,
		eUsage_DoNotClip = D3DUSAGE_DONOTCLIP,
		eUsage_Points = D3DUSAGE_POINTS,
		eUsage_RTPatches = D3DUSAGE_RTPATCHES,
		eUsage_NPatches = D3DUSAGE_NPATCHES,
	};

	// lock
	enum eLock
	{
		eLock_Null = 0, 
		eLock_ReadOnly = D3DLOCK_READONLY,
		eLock_Discard = D3DLOCK_DISCARD,
		eLock_NoOverWrite = D3DLOCK_NOOVERWRITE,
		eLock_NoSysLock = D3DLOCK_NOSYSLOCK,
		eLock_DoNotWait = D3DLOCK_DONOTWAIT,
		eLock_No_Dirty_Update = D3DLOCK_NO_DIRTY_UPDATE,
	};

	// Primitives supported by draw-primitive API
	enum ePrimitive
	{
		ePrimitive_PointList = D3DPT_POINTLIST,
		ePrimitive_LineList = D3DPT_LINELIST,
		ePrimitive_LineStrip = D3DPT_LINESTRIP,
		ePrimitive_TriangleList = D3DPT_TRIANGLELIST,
		ePrimitive_TriangleStrip = D3DPT_TRIANGLESTRIP,
		ePrimitive_TriangleFan = D3DPT_TRIANGLEFAN,
	};

	//
#define StreamSource_IndexedData D3DSTREAMSOURCE_INDEXEDDATA
#define StreamSource_InstanceData D3DSTREAMSOURCE_INSTANCEDATA

	//
	enum eFormat
	{
		//
		eFormat_Index16 = D3DFMT_INDEX16,

		//
		eFormat_A8 = D3DFMT_A8,
		eFormat_A8R8G8B8 = D3DFMT_A8R8G8B8,
		eFormat_X8R8G8B8 = D3DFMT_X8R8G8B8,
	};

	//
	enum eRenderState
	{
		eRenderState_FillMode = D3DRS_FILLMODE,
	};

	//
	enum eFillMode
	{
		eFillMode_Point = D3DFILL_POINT,
		eFillMode_WireFrame = D3DFILL_WIREFRAME,
		eFillMode_Solid = D3DFILL_SOLID,
		eFillMode_Size,
	};

	//
	typedef D3DVIEWPORT9 sViewPort;

	//
	class Surface
	{
	public:
		IDirect3DSurface9* getSurface();
	private:
		
	};
	//
	class EDX : public Buddha::Exception
	{
	public:
		/** Constructor
			* You can provide an internet address. If so, reason *must* contain "%s"
			* where the address should be written. Moreover, the length of reason plus
			* the length of the address when displayed by asString() should no exceed 256.
			*/
		EDX( HRESULT r)
		{
			std::ostringstream ss;
			ss<<"dx error, string: "<<DXGetErrorString(r)<<";description: "<<DXGetErrorDescription(r);
			_reason = ss.str();
		}
	};
}

#endif // __RenderEngineCreationParameters_h__
