#include "stdafx.h"
#include "SceneNode.h"
#include "ISceneManager.h"
#include "D3D9Mapping.h"
#include "Paras.h"
#include "Camera.h"
#include "IRenderSystem.h"
//
namespace Euclid
{
	SceneNode::SceneNode(ISceneManager* sceneManager, const std::string& name)
	{
		_sceneManager = sceneManager;
		_name = name;
	}

	SceneNode::~SceneNode()
	{

	}

	void SceneNode::attachObject( RenderableObject* object )
	{
		if (object)
		{
			_attachedObjects[object->getName()] = object;
			object->attachNode(this);
			_sceneManager->onAttachObject(object);
		}
	}

	RenderableObject* SceneNode::getAttachedObject(const std::string& name)
	{
		AttachedObjectsMap::iterator it = _attachedObjects.find(name);
		if (it != _attachedObjects.end())
		{
			return it->second;
		}
	}

	void SceneNode::detachObject(const std::string& name)
	{
		AttachedObjectsMap::iterator it = _attachedObjects.find(name);
		if (it != _attachedObjects.end())
		{
			_attachedObjects.erase(it);
		}
	}

	void SceneNode::render(eRenderQueueOrder order)
	{
		//
		{
			AttachedObjectsMap::iterator it;
			for (it = _attachedObjects.begin(); it != _attachedObjects.end(); ++it)
			{
				if (it->second->isVisible() && it->second->getRenderQueueOrder() == order)
				{
					//it->second->attachNode(this);
					//it->second->preRender();
					it->second->render();
					//it->second->postRender();
				}
			}
		}
		
		//
		{		
			NameNodeMap::iterator it;
			for (it = _children.begin(); it != _children.end(); ++it)
			{
				SceneNode* n = static_cast<SceneNode*>(it->second);
				n->render(order);
			}
		}
	}

	SceneNode* SceneNode::addSceneNodeChild( const std::string& name )
	{
		return static_cast<SceneNode*>(this->addChild(name));
	}

	Node* SceneNode::createChild( const std::string& name )
	{
		if (_sceneManager)
		{
			return _sceneManager->createSceneNode(name);
		}
	}

	void SceneNode::detachAllObjects()
	{
		AttachedObjectsMap::iterator i = _attachedObjects.begin();
		for (; i != _attachedObjects.end();++i)
		{
			i->second->destroy();
			_sceneManager->onDettachObject(i->second);
			delete i->second;
			i->second = NULL;
		}

		_attachedObjects.clear();

// 		NameNodeMap::iterator it;
// 		for (it = _children.begin(); it != _children.end(); ++it)
// 		{
// 			SceneNode* n = static_cast<SceneNode*>(it->second);
// 			n->detachAllObjects();
// 		}
	}

	void SceneNode::update(u32 current, u32 delta)
	{
		//
		AttachedObjectsMap::iterator it;
		for (it = _attachedObjects.begin(); it != _attachedObjects.end(); ++it)
		{
			it->second->update(current, delta);
		}

		//
		NameNodeMap::iterator iu;
		for (iu = _children.begin(); iu != _children.end(); ++iu)
		{
			SceneNode* n = static_cast<SceneNode*>(iu->second);
			n->update(current, delta);
		}
	}

	Mat4 SceneNode::getWorldViewProjectMatrix()
	{
// 		needUpdate();
// 		D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_getFullTransform());
// 		D3DXMATRIX v;
// 		Paras::getInstancePtr()->_camera->getViewMatrix(v);
// 		D3DXMATRIX p;
// 		D3DVIEWPORT9 vp;
// 		Paras::getInstancePtr()->_renderSystem->getViewport(&vp);
// 		float width = vp.Width;
// 		float height = vp.Height;
// 		D3DXMatrixPerspectiveFovRH(&p, D3DX_PI * 0.45, width / height, 0.1f, 10000.f);
// 		Mat4 r = D3D9Mapping::convertD3DXMatrix(m * v * p);
		return Mat4::IDENTITY;
	}

}
