//==========================================================================
/**
* @file	  : Common.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-24   21:15
* purpose : 
*/
//==========================================================================

#ifndef __EuclidCommon_h__
#define __EuclidCommon_h__

//
#include "base/Buddha.h"
#include "math/Euler.h"

//
namespace Euclid
{
	//
#if defined(render_EXPORTS) // inside DLL
#   define _EuclidExport_   __declspec(dllexport)
#else // outside DLL
#   define _EuclidExport_   __declspec(dllimport)
#endif

	//
	/* Vector */
	typedef float vec3[3];

	class VertexBuffer;
	class IndexBuffer;
	class IBufferFactory;
	class BufferFactory;
	class Material;
	class Light;
	class Camera;
	class CameraEx;
	class CameraControllerThirdPerson;
	class IShader;
	class VertexShader;
	class PixelShader;
	class IShaderFactory;
	class ShaderFactory;
	class ITexture;
	class Texture;
	class IRenderSystem;
	class IFont;
	class IFontFactory;
	class FontFactory;
	class FreeType;
	class RenderSystem;
	class ITextureFactory;
	class TextureFactory;
	class Paras;

	//
	class IRenderEngine;
	class RenderEngine;

	//
	class Node;
	class SceneNode;
	class LightNode;
	class RenderableObject;
	class RenderablePara;
	class RenderableObjectFactory;
	class ManualObject;
	class ManualPara;
	class LightningStrip;
	class Spiral;
	class Blooding;
	class ManualObjectFactory;
	class SkyBox;
	class SkyBoxFactory;
	class Background;
	class BackgroundPara;
	class BackgroundFactory;
	class Terrain;
	class EntityX;
	class EntityXFactory;
	class EntityMD2;
	class EntityMD2Factory;
	class ISceneManager;
	class SceneManager;

	//
	class D3D9Mapping;
	class MD2Loader;
	class MD3Loader;
	class BSPLoader;

	class MPQLoader;

	//
	class PK3Archive;
	class IFileRead;
	class MemoryFileReader;
	class IZIPManager;
	class ZIPManager;

	//
	class BillboardPara;
	class Billboard;
	class BillboardFactory;

	//
	class ParticlePara;
	class Particle;
	class ParticleSystemPara;
	class ParticleSystem;

	//
	class Color;

	//
	class EntityYZ;
	class EntityYZPara;
	class EntityYZMaterial;
	class EntityYZFactory;
	class EntityYZInstance;
	class SubEntityYZInstance;
	class EntityYZInstancePara;

	//
	class BoneNode;
	class Skeleton;

	//
	class ParticleEmitter;

	//
	class IEffect;
	class IEffectFactory;
}

//
#endif // __EuclidCommon_h__
