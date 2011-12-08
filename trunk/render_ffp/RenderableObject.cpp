#include "RenderableObject.h"
#include "Node.h"
#include "Paras.h"
#include "IShader.h"
#include "IEffect.h"
#include "IRenderSystem.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
//
namespace Euclid
{
	RenderableObject::RenderableObject()
		:	_sceneManager(NULL), _type(""), _isVisible(true), _currentNode(NULL), _vertexShader(NULL), _pixelShader(NULL), _effect(NULL)
		, _vdt(VD_NULL), _vb(NULL), _ib(NULL), _orthoProjection(false)
	{

	}

	RenderableObject::RenderableObject( const std::string& name )
		:	_sceneManager(NULL), _type(""), _isVisible(true), _currentNode(NULL), _vertexShader(NULL), _pixelShader(NULL), _effect(NULL)
		, _vdt(VD_NULL), _vb(NULL), _ib(NULL), _orthoProjection(false)
	{
		_name = name;
		_effect = NULL;
	}

	RenderableObject::~RenderableObject()
	{
		destroy();
	}

	void RenderableObject::preRender()
	{

	}

	void RenderableObject::postRender()
	{

	}

	void RenderableObject::destroy()
	{
		if (_vertexShader)
		{
			_vertexShader->destroy();
			delete _vertexShader;
			_vertexShader = NULL;
		}

		if (_pixelShader)
		{
			_pixelShader->destroy();
			delete _pixelShader;
			_pixelShader = NULL;
		}

		if (_effect)
		{
			_effect->release(this);
			_effect = NULL;
		}

		if (_vb)
		{
			_vb->destroy();
			_vb = NULL;
		}

		if (_ib)
		{
			_ib->destroy();
			_ib = NULL;
		}
	}

	std::string RenderableObject::getName() const
	{
		return _name;
	}

	void RenderableObject::attachNode( Node* node )
	{
		_currentNode = node;
	}

	void RenderableObject::setName( const std::string& name )
	{
		_name = name;
	}

	void RenderableObject::setType( std::string& type )
	{
		_type = type;
	}

	const std::string& RenderableObject::getType() const
	{
		return _type;
	}

	void RenderableObject::update(u32 current, u32 delta)
	{
	}

	bool RenderableObject::isVisible()
	{
		return _isVisible;
	}

	void RenderableObject::setVisible(bool visible)
	{
		_isVisible = visible;
	}

	bool RenderableObject::create()
	{
		return true;
	}

	void RenderableObject::setVertexShader( IShader* s )
	{
		_vertexShader = s;
	}

	void RenderableObject::setPixelShader( IShader* s )
	{
		_pixelShader = s;
	}

	void RenderableObject::setEffect( IEffect* effect )
	{
		effect->onSetEffect(this);
		_effect = effect;
	}

	Euclid::eRenderType RenderableObject::getRenderType()
	{
		if (_effect)
		{
			return eRenderType_Effect;
		}

		if (_vertexShader || _pixelShader)
		{
			return eRenderType_Shader;
		}

		return eRenderType_Normal;
	}

	void RenderableObject::renderWithEffect()
	{

	}

	void RenderableObject::renderNormal()
	{

	}

	void RenderableObject::render()
	{
		switch(getRenderType())
		{
		case eRenderType_Normal:
			{
				preRender();
				renderNormal();
				postRender();
			}break;

		case eRenderType_Shader:
			{
				preRender();
				if (_vertexShader)
				{
					_renderSystem->setVertexShader(_vertexShader);
				}

				if (_pixelShader)
				{
					_renderSystem->setPixelShader(_pixelShader);
				}
				
				renderNormal();

				if (_vertexShader)
				{
					_renderSystem->setVertexShader(NULL);
				}

				if (_pixelShader)
				{
					_renderSystem->setPixelShader(NULL);
				}
				postRender();
			}break;

		case eRenderType_Effect:
			{
				renderWithEffect();	
			}break;
		}
	}

	IEffect* RenderableObject::getEffect()
	{
		return _effect;
	}

	void RenderableObject::setVertexDeclarationType( eVertexDeclarationType t )
	{
		_vdt = t;
	}

	void RenderableObject::setOrthoProjection( bool b )
	{
		_orthoProjection = b;
	}

	bool RenderableObject::isOrthoProjection()
	{
		return _orthoProjection;
	}

	Euclid::eRenderQueueOrder RenderableObject::getRenderQueueOrder()
	{
		return _order;
	}

	void RenderableObject::setRenderQueueOrder( eRenderQueueOrder order )
	{
		_order = order;
	}

	RenderSpace* RenderableObject::getRenderSpaceVector()
	{
		return &_space;
	}

	//IRenderSystem* RenderableObject::_renderSystem( Paras::getInstancePtr()->_renderSystem);

	IRenderSystem* RenderableObject::_renderSystem(NULL);
}
