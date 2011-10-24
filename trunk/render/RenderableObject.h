//==========================================================================
/**
* @file	  : RenderableObject.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   19:55
* purpose : 
*/
//==========================================================================

#ifndef __RenderableObject_h__
#define __RenderableObject_h__

#include "Common.h"
#include "VertexDeclaration.h"
#include "Appearance.h"
//
namespace Euclid
{
	enum eRenderType
	{
		eRenderType_Normal,
		eRenderType_Shader,
		eRenderType_Effect,
	};

	class _EuclidExport_ RenderablePara
	{
	};

	class _EuclidExport_ RenderableObject
	{
	public:
		/// Constructor
        RenderableObject();

		/// Named constructor
		RenderableObject(const std::string& name);
        /** Virtual destructor - read Scott Meyers if you don't know why this is needed.
        */
        virtual ~RenderableObject();

		//
	public:
		//
		virtual void update(u32 current, u32 delta);
		virtual bool create();
		virtual void destroy();

		virtual void attachNode(Node* node);
		virtual void preRender();
		virtual void postRender();
		virtual void setVertexShader(IShader* s);
		virtual void setPixelShader(IShader* s);
		
		
		IEffect* getEffect();
		virtual void setEffect(IEffect* effect);
		//
		virtual void render();
		virtual void renderWithEffect();
		virtual void renderNormal();

		//
		std::string getName() const;

		//
		void setName(const std::string& name);

		//
		void setType(std::string& type);
		const std::string& getType() const;

		//
		bool isVisible();
		void setVisible(bool visible);
		//
		void setOrthoProjection(bool b);
		bool isOrthoProjection();
		//
		eRenderType getRenderType();

		//
		IShader* getVertexShader()
		{
			return _vertexShader;
		}

		//
		IShader* getPixelShader()
		{
			return _pixelShader;
		}

		void setVertexDeclarationType(eVertexDeclarationType t);
		//
		eRenderQueueOrder getRenderQueueOrder();
		void setRenderQueueOrder(eRenderQueueOrder order);
		//
		RenderSpace* getRenderSpaceVector();
		//
		bool operator < (RenderableObject* rhs)
		{
			return _space.difference(RenderSpace::tZero).size() < rhs->getRenderSpaceVector()->difference(RenderSpace::tZero).size();
		}
		//
	protected:
		//	Name of this object
		std::string			_name;

		//	Mesh ? Manual ? Light ? Camera ?
		std::string			_type;

		//	SceneManager holding this object (if applicable)
		ISceneManager*		_sceneManager;

		//
		Node*				_currentNode;

		static IRenderSystem*		_renderSystem;

		//
		bool				_isVisible;
		IndexBuffer*				_ib;
		VertexBuffer*				_vb;
		IShader*			_vertexShader;
		IShader*			_pixelShader;
		IEffect*			_effect;
		eVertexDeclarationType	_vdt;
		bool				_orthoProjection;
		eRenderQueueOrder	_order;
		RenderSpace			_space;
	};
}

#endif // __RenderableObject_h__
 
