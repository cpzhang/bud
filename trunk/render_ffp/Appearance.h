//==========================================================================
/**
* @file	  : Appearance.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-30   17:23
* purpose : 
*/
//==========================================================================

#ifndef __Appearance_h__
#define __Appearance_h__
#include "Paras.h"
#include "VertexDeclaration.h"
#include "IRenderSystem.h"
namespace Euclid
{
	class RenderSpaceComponent
	{
	public:
		RenderSpaceComponent(const std::string& rName)
			:_name(rName)
		{

		}
		std::string _name;
		virtual bool equal(RenderSpaceComponent* rs) = NULL;
		virtual bool apply() = NULL;
		virtual void setVaule(unsigned long value){};
		virtual void setVaule(eVertexDeclarationType value){};
		virtual void setVaule(Mat4 value){};
		virtual void setVaule(VertexBuffer* value, unsigned int stride, size_t vertexCount){};
		virtual void setVaule(ITexture* value){};
		virtual void setVaule(IndexBuffer* ib){};
	};
	class RenderSpaceComponent_RenderState : public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_RenderState(const std::string& rName, D3DRENDERSTATETYPE rtype, unsigned long rValue)
			:RenderSpaceComponent(rName), _type(rtype), _value(rValue)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_RenderState* rsc = static_cast<RenderSpaceComponent_RenderState*>(rs);
			return rsc->_value == _value;
		}
		virtual bool apply()
		{
			return Paras::getInstancePtr()->_renderSystem->setRenderState(_type, _value);
		}
		virtual void setVaule(unsigned long value)
		{
			_value = value;
		};
		unsigned long _value;
		D3DRENDERSTATETYPE	_type;
	};
	class RenderSpaceComponent_VertexDeclaration : public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_VertexDeclaration(const std::string& rName, eVertexDeclarationType	rVdt)
			:RenderSpaceComponent(rName), _value(rVdt)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_VertexDeclaration* rsc = static_cast<RenderSpaceComponent_VertexDeclaration*>(rs);
			return rsc->_value == _value;
		}
		virtual bool apply()
		{
			return Paras::getInstancePtr()->_renderSystem->setVertexDeclaration(_value);
		}
		virtual void setVaule(eVertexDeclarationType value)
		{
			_value = value;
		};
		eVertexDeclarationType	_value;
	};

	class RenderSpaceComponent_Transform: public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_Transform(const std::string& rName, Mat4& rValue)
			:RenderSpaceComponent(rName), _value(rValue)
		{

		}
		RenderSpaceComponent_Transform(const std::string& rName, const Mat4& rValue)
			:RenderSpaceComponent(rName), _value(rValue)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_Transform* rsc = static_cast<RenderSpaceComponent_Transform*>(rs);
			return rsc->_value == _value;
		}
		virtual void setVaule(Mat4 value)
		{
			_value = value;
		};
		Mat4	_value;
	};
	class RenderSpaceComponent_WorldTransform: public RenderSpaceComponent_Transform
	{
	public:
		RenderSpaceComponent_WorldTransform(const std::string& rName, Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		RenderSpaceComponent_WorldTransform(const std::string& rName, const Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		virtual bool apply()
		{
			return Paras::getInstancePtr()->_renderSystem->setWorldMatrix(_value);
		}
	};
	class RenderSpaceComponent_ViewTransform: public RenderSpaceComponent_Transform
	{
	public:
		RenderSpaceComponent_ViewTransform(const std::string& rName, Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		RenderSpaceComponent_ViewTransform(const std::string& rName, const Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		virtual bool apply()
		{
			return Paras::getInstancePtr()->_renderSystem->setViewMatrix(_value);
		}
	};
	class RenderSpaceComponent_ProjectTransform: public RenderSpaceComponent_Transform
	{
	public:
		RenderSpaceComponent_ProjectTransform(const std::string& rName, Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		RenderSpaceComponent_ProjectTransform(const std::string& rName, const Mat4& rValue)
			:RenderSpaceComponent_Transform(rName, rValue)
		{

		}
		virtual bool apply()
		{
			return Paras::getInstancePtr()->_renderSystem->setProjectionMatrix(_value);
		}
	};
	class RenderSpaceComponent_VertexBuffer: public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_VertexBuffer(const std::string& rName, VertexBuffer* rVB, unsigned int stride, size_t vertexCount)
			:RenderSpaceComponent(rName), _vb(rVB), _stride(stride), _vertexCount(vertexCount)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_VertexBuffer* rsc = static_cast<RenderSpaceComponent_VertexBuffer*>(rs);
			return rsc->_vb == _vb && rsc->_stride == _stride;
		}
		virtual bool apply()
		{
			if (_vb)
			{
				return Paras::getInstancePtr()->_renderSystem->setStreamSource(0, _vb, 0, _stride);
			}
			return true;
		}
		virtual void setVaule(VertexBuffer* vb, unsigned int stride, size_t vertexCount)
		{
			_vb = vb;
			_stride = stride;
			_vertexCount = vertexCount;
		};
		VertexBuffer*	_vb;
		unsigned int	_stride;
		size_t			_vertexCount;
	};
	class RenderSpaceComponent_IndexBuffer: public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_IndexBuffer(const std::string& rName, IndexBuffer* rIB, unsigned int stride)
			:RenderSpaceComponent(rName), _ib(rIB), _stride(stride)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_IndexBuffer* rsc = static_cast<RenderSpaceComponent_IndexBuffer*>(rs);
			return rsc->_ib == _ib;
		}
		virtual bool apply()
		{
			if (_ib)
			{
				return Paras::getInstancePtr()->_renderSystem->setIndices(_ib);
			}
			return true;
		}
		virtual void setVaule(IndexBuffer* ib)
		{
			_ib = ib;
		};
		IndexBuffer*	_ib;
		unsigned int	_stride;
	};
	class RenderSpaceComponent_Texture: public RenderSpaceComponent
	{
	public:
		RenderSpaceComponent_Texture(const std::string& rName, ITexture* rValue)
			:RenderSpaceComponent(rName), _textures(rValue)
		{

		}
		virtual bool equal(RenderSpaceComponent* rs)
		{
			RenderSpaceComponent_Texture* rsc = static_cast<RenderSpaceComponent_Texture*>(rs);
			return rsc->_textures == _textures;
		}
		virtual bool apply()
		{
			if (_textures)
			{

				Paras::getInstancePtr()->_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				Paras::getInstancePtr()->_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			}
			return 	Paras::getInstancePtr()->_renderSystem->setTexture(0, _textures);
		}
		virtual void setVaule(ITexture* value)
		{
			_textures = value;
		};
		ITexture*	_textures;
	};
	class _EuclidExport_ RenderSpace
	{
	public:
		RenderSpace();
		~RenderSpace();
		std::vector<RenderSpaceComponent*> _components;
		static RenderSpace tZero;
		//
		std::vector<size_t>& difference(const RenderSpace& rs); 
		RenderSpaceComponent* getComponent(const std::string& rName);
		//
		void render();
	};
	/** 渲染队列的排序优先级 */
	enum eRenderQueueOrder
	{
		eRenderQueueOrder_Line,
		eRenderQueueOrder_Texture,
		eRenderQueueOrder_Size,
	};

	/** Blend modes for models / particle systems / ribbon system. */
	enum eBlendMode
	{
		BM_OPAQUE,
		BM_TRANSPARENT,
		BM_ALPHA_BLEND,
		BM_ADDITIVE,
		BM_ADDITIVE_ALPHA,
		BM_MODULATE,
		BM_MODULATE2X
	};

	/** Hardware culling modes based on vertex winding.
	This setting applies to how the hardware API culls triangles it is sent. */
	enum eCullingMode
	{
		/// Hardware never culls triangles and renders everything it receives.
		CULL_NONE = 1,
		/// Hardware culls triangles whose vertices are listed clockwise in the view (default).
		CULL_CLOCKWISE = 2,
		/// Hardware culls triangles whose vertices are listed anticlockwise in the view.
		CULL_COUNTERCLOCKWISE = 3
	};

	/** Blending factors for manually blending objects with the scene. If there isn't a predefined
	SceneBlendType that you like, then you can specify the blending factors directly to affect the
	combination of object and the existing scene. See Material::setSceneBlending for more details.
	*/
	enum eSceneBlendFactor
	{
		SBF_ONE,
		SBF_ZERO,
		SBF_DEST_COLOR,
		SBF_SOURCE_COLOR,
		SBF_ONE_MINUS_DEST_COLOR,
		SBF_ONE_MINUS_SOURCE_COLOR,
		SBF_DEST_ALPHA,
		SBF_SOURCE_ALPHA,
		SBF_ONE_MINUS_DEST_ALPHA,
		SBF_ONE_MINUS_SOURCE_ALPHA
	};

	/** Texture addressing mode. */
	enum eTextureAddressingMode
	{
		/// Texture wraps at values over 1.0
		TAM_WRAP,
		/// Texture mirrors (flips) at joins over 1.0
		TAM_MIRROR,
		/// Texture clamps at 1.0
		TAM_CLAMP_TO_EDGE,
		//
		TAM_CLAMP
	};

	/// Enum describing the ways to generate texture coordinates
	enum eTexCoordCalcMethod
	{
		/// No calculated texture coordinates
		TEXCALC_NONE,
		/// Environment map based on vertex normals
		TEXCALC_ENVIRONMENT_MAP,
		/// Environment map based on vertex positions
		TEXCALC_ENVIRONMENT_MAP_PLANAR,
		TEXCALC_ENVIRONMENT_MAP_REFLECTION,
		TEXCALC_ENVIRONMENT_MAP_NORMAL,
		/// Projective texture
		TEXCALC_PROJECTIVE_TEXTURE
	};

	/// 常用的贴图混合模式
	enum eTextureBlending
	{
		TB_NONE,		//关闭贴图混合
		TB_ALPHAMAP,	//0 - AlphaMap 1 - Texture，一般用于地表渲染
		TB_DIFFUSEMAP,	//使用Diffuse颜色值的Alpha混合贴图，类似于OpenGL中的顶点Alpha
		TB_TFACTOR,		//使用颜色来模拟混合调色，以取代用光照实现的调色
		TB_DIFFUSECOLORMAP,	//使用Diffuse颜色值、TFactor颜色以及贴图来获得最终效果
		TB_TEXTUREFLOWMAP	//stage1的贴图流动
	};

	struct Appearance
	{
		struct Geometry
		{
			VertexBuffer*			_vb;// Position
			u8						_vStride;
			VertexBuffer*			_tb;// Texture
			u8						_tStride;
			u16						_bStart, _bCount;

			IndexBuffer*			_ib;
			u16						_iStart, _iCount;

			eVertexDeclarationType	_vd;
		};

		struct Material
		{
			eRenderQueueOrder		_order;
			eBlendMode				_bm;
			Color					_diffuse;
			eCullingMode			_cm;
			eSceneBlendFactor		_srcSBF;
			eSceneBlendFactor		_desSBF;
			bool					_isAlphaCheckEnable;
			bool					_isLightingEnable;
			bool					_isFogEnable;
			bool					_isDepthWrite;
			bool					_isDepthCheck;
			eTextureAddressingMode	_tamS;
			eTextureAddressingMode	_tamT;
			eTexCoordCalcMethod		_tccm;
			ITexture*				_tex0;
			bool					_texFlow0;
			Mat4					_texMat0;
			ITexture*				_tex1;
			bool					_texFlow1;
			Mat4					_texMat1;
			eTextureBlending		_tb;
		};

		struct Transform
		{
			Mat4	_worldTransform;
		};

		Geometry	_geometry;
		Material	_material;
		Transform	_transform;
	};
}


#endif // __Appearance_h__
 
