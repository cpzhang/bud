#include "stdafx.h"
#include "SceneManager.h"
#include "ManualObjectFactory.h"
#include "RenderableObjectFactory.h"
#include "SkyBoxFactory.h"
#include "SceneNode.h"
#include "EntityX.h"
#include "EntityMD2.h"
#include "ManualObject.h"
#include "SkyBox.h"
#include "Light.h"
//#include "LightNode.h"
#include "EntityXFactory.h"
#include "EntityMD2Factory.h"
#include "Blooding.h"
#include "BackgroundFactory.h"
#include "Background.h"
#include "Billboard.h"
#include "BillboardFactory.h"
#include "ParticleSystem.h"
#include "ParticleSystemFactory.h"
#include "EntityYZ.h"
#include "EntityYZFactory.h"
#include "EntityYZInstance.h"
#include "EntityYZInstanceFactory.h"
#include "Paras.h"
#include "CameraEx.h"
#include "CameraControllerThirdPerson.h"
#include "IRenderSystem.h"
//
namespace Euclid
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{

	}

	EntityX* SceneManager::createEntityX( const std::string& EntityXName, const std::string& meshName )
	{
		//
		RenderableObject* o = EntityXFactory::getInstancePtr()->createInstanceImpl(EntityXName, meshName);

		//
		return static_cast<EntityX*>(o);
	}


	EntityMD2* SceneManager::createEntityMD2( const std::string& EntityMD2Name, const std::string& md2Name )
	{
		//
		RenderableObject* o = EntityMD2Factory::getInstancePtr()->createInstanceImpl(EntityMD2Name, md2Name);

		//
		return static_cast<EntityMD2*>(o);
	}


	Euclid::ManualObject* SceneManager::createManualObject( const std::string& name)
	{
		//
		if (ManualObjectFactory::getInstancePtr() == NULL)
		{
			new ManualObjectFactory;
		}

		//
		RenderableObject* o = ManualObjectFactory::getInstancePtr()->createInstanceImpl(name);

		//
		return static_cast<ManualObject*>(o);
	}

	ManualObject* SceneManager::createManualObject( ManualPara& para )
	{
		//
		if (ManualObjectFactory::getInstancePtr() == NULL)
		{
			new ManualObjectFactory;
		}

		//
		RenderableObject* o = ManualObjectFactory::getInstancePtr()->createInstanceImpl(para);

		//
		return static_cast<ManualObject*>(o);
	}

	SkyBox* SceneManager::createSkyBox( const std::string& name, const std::string& file )
	{
		//
		RenderableObject* o = SkyBoxFactory::getInstancePtr()->createInstanceImpl(name, file);

		//
		return static_cast<SkyBox*>(o);
	}

	SceneNode* SceneManager::getRootSceneNode()
	{
		SceneNodeMap::iterator it = _nodes.find(_RootSceneNode);
		if (_nodes.end() == it)
		{
			_nodes[_RootSceneNode] = new SceneNode(this, _RootSceneNode);
		}

		//
		return _nodes[_RootSceneNode];
	}

	SceneNode* SceneManager::createSceneNode(const std::string& name)
	{
		SceneNodeMap::iterator it = _nodes.find(name);
		if (_nodes.end() == it)
		{
			_nodes[name] = new SceneNode(this, name);
		}

		//
		return _nodes[name];
	}

	void SceneManager::render(eRenderQueueOrder order)
	{
		////
		//{
		//	LightNodeMap::iterator it = _lights.find(_RootLightNode);
		//	if (_lights.end() != it)
		//	{
		//		it->second->render();
		//	}
		//}

		//
		if (0)
		{
			for (size_t i = 0; i != eRenderQueueOrder_Size; ++i)
			{
				SceneNodeMap::iterator it = _nodes.find(_RootSceneNode);
				if (_nodes.end() != it)
				{
					it->second->render((eRenderQueueOrder)i);
				}
			}
		}
		else
		{
			if (0)
			{
				std::sort(_renderableObjects.begin(), _renderableObjects.end());
				RenderSpace* rs_last = &RenderSpace::tZero;
				{
					Mat4 viewMat = Paras::getInstancePtr()->_cameraEx->getViewMatrix();
					Mat4 controllerMat = Mat4::IDENTITY;
					if (Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson())
					{
						controllerMat = Paras::getInstancePtr()->_cameraEx->getControllerThirdPerson()->getMatrix();
					}

					Mat4 projectMat = Paras::getInstancePtr()->_cameraEx->getProjectionMatrix();
					rs_last->getComponent("view transformation")->setVaule(viewMat * controllerMat);
					rs_last->getComponent("project transformation")->setVaule(projectMat);
				}
				for (size_t i = 0; i != rs_last->_components.size(); ++i)
				{
					rs_last->_components[i]->apply();
				}
				for (size_t i = 0; i != _renderableObjects.size(); ++i)
				{
					if (!_renderableObjects[i]->isVisible())
					{
						continue;
					}
					RenderSpace* rs_current = _renderableObjects[i]->getRenderSpaceVector();
					std::vector<size_t>& ds = rs_current->difference(*rs_last);
					for (size_t k = 0; k != ds.size(); ++k)
					{
						if (k != 8 && k != 9)
						{
							rs_current->_components[ds[k]]->apply();
						}
					}

					//
					_renderableObjects[i]->renderNormal();

					//
					rs_last = rs_current;
				}
			}
			else
			{
				sortRenderableObjects();
				RenderSpace* rs_last = NULL;
				for(size_t i = 0; i != _sortedRenderableObjects.size(); ++i)
				{
					size_t index = _sortedRenderableObjects[i];
					RenderSpace* rs_current = _renderableObjects[index]->getRenderSpaceVector();
					if (i == 0)
					{
						for (size_t j = 0; j != rs_current->_components.size(); ++j)
						{
							rs_current->_components[j]->apply();
						}
					}
					else
					{
						std::vector<size_t>& ds = rs_current->difference(*rs_last);
						for (size_t k = 0; k != ds.size(); ++k)
						{
							//if (k != 8 && k != 9)
							{
								rs_current->_components[ds[k]]->apply();
							}
						}
						//
						_renderableObjects[index]->renderNormal();
					}
					rs_last = rs_current;
				}
			}
		}
	}

	Light* SceneManager::createLight( const std::string& name )
	{
		return new Light;
	}


	//LightNode* SceneManager::getRootLightNode()
	//{
	//	LightNodeMap::iterator it = _lights.find(_RootLightNode);
	//	if (_lights.end() == it)
	//	{
	//		_lights[_RootLightNode] = new LightNode(this, _RootLightNode);
	//	}

	//	//
	//	return _lights[_RootLightNode];
	//}

	//LightNode* SceneManager::createLightNode( const std::string& name )
	//{
	//	LightNodeMap::iterator it = _lights.find(name);
	//	if (_lights.end() == it)
	//	{
	//		_lights[name] = new LightNode(this, name);
	//	}

	//	//
	//	return _lights[name];
	//}

	SceneNode* SceneManager::getSceneNode( const std::string& name )
	{
		if (_nodes.find(name) == _nodes.end())
		{
			return NULL;
		}

		return _nodes[name];
	}


	void SceneManager::_destroySceneNodeAndChildren( const std::string& name )
	{
		SceneNodeMap::iterator it = _nodes.find(name);
		if (_nodes.end() != it)
		{
			SceneNode* n = it->second;
			if (n)
			{
				if (n->getChildrenSize() == 0)
				{
				}
				else
				{
					Node::NameNodeMap::iterator ie;
					for (ie = n->_children.begin(); ie != n->_children.end(); ++ie)
					{
						_destroySceneNodeAndChildren(ie->first);
					}
				}

				n->detachAllObjects();
				//n->getParent()->eraseChild(it->first);
				n->release();
				n = NULL;
				_nodes.erase(it);
			}
		}
	}

	void SceneManager::destroySceneNodeAndChildren( const std::string& name )
	{
		SceneNodeMap::iterator it = _nodes.find(name);
		if (_nodes.end() == it)
		{
			return;
		}
		SceneNode* n = it->second;
		if (n)
		{
			Node* p = n->getParent();
			if (p)
			{
				p->eraseChild(name);
			}
		}

		_destroySceneNodeAndChildren(name);
	}

	void SceneManager::destroyAllSceneNodes()
	{
		destroySceneNodeAndChildren(_RootSceneNode);
	}

	void SceneManager::update(u32 current, u32 delta)
	{
		SceneNodeMap::iterator it = _nodes.find(_RootSceneNode);
		if (_nodes.end() != it)
		{
			it->second->update(current, delta);
		}
	}

	Background* SceneManager::createBackground( BackgroundPara& para )
	{
		//
		RenderableObject* o = BackgroundFactory::getInstancePtr()->createInstanceImpl(para);

		//
		return static_cast<Background*>(o);
	}

	Billboard* SceneManager::createBillboard( BillboardPara* para )
	{
		//
		RenderableObject* o = BillboardFactory::getInstancePtr()->createInstanceImpl(para);

		//
		return static_cast<Billboard*>(o);
	}

	ParticleSystem* SceneManager::createParticleSystem( ParticleSystemPara* para )
	{
		//
		RenderableObject* o = ParticleSystemFactory::getInstancePtr()->createInstanceImpl(para);

		//
		return static_cast<ParticleSystem*>(o);
	}

	EntityYZ* SceneManager::createEntityYZ( EntityYZPara* p )
	{
		if(EntityYZFactory::getInstancePtr() == NULL)
		{
			new EntityYZFactory;
		}
		//
		RenderableObject* o = EntityYZFactory::getInstancePtr()->createInstanceImpl(p);

		//
		return static_cast<EntityYZ*>(o);
	}

	bool SceneManager::create()
	{
		new BackgroundFactory;
		new EntityXFactory;
		_renderableObjects.push_back(new RenderableObject);
		return true;
	}

	void SceneManager::destroy()
	{
		//
		destroyAllSceneNodes();

		//
		if (BackgroundFactory::getInstancePtr())
		{
			delete BackgroundFactory::getInstancePtr();
		}

		//
		if (ManualObjectFactory::getInstancePtr())
		{
			delete ManualObjectFactory::getInstancePtr();
		}

		//
		if (EntityXFactory::getInstancePtr())
		{
			delete EntityXFactory::getInstancePtr();
		}

		//
		if (EntityYZFactory::getInstancePtr())
		{
			delete EntityYZFactory::getInstancePtr();
		}

		//
		if (EntityYZInstanceFactory::getInstancePtr())
		{
			delete EntityYZInstanceFactory::getInstancePtr();
		}
	}

	EntityYZInstance* SceneManager::createEntityYZInstance( EntityYZInstancePara* p )
	{
		if(EntityYZInstanceFactory::getInstancePtr() == NULL)
		{
			new EntityYZInstanceFactory;
		}
		//
		RenderableObject* o = EntityYZInstanceFactory::getInstancePtr()->createInstanceImpl(p);

		//
		return static_cast<EntityYZInstance*>(o);
	}

	void SceneManager::onAttachObject( RenderableObject* o )
	{
		std::vector<RenderableObject*>::iterator it = std::find(_renderableObjects.begin(), _renderableObjects.end(), o);
		if (it == _renderableObjects.end())
		{
			_renderableObjects.push_back(o);
		}
	}

	void SceneManager::onDettachObject( RenderableObject* o )
	{
		std::vector<RenderableObject*>::iterator it = std::find(_renderableObjects.begin(), _renderableObjects.end(), o);
		if (it != _renderableObjects.end())
		{
			_renderableObjects.erase(it);
		}
	}

	void SceneManager::sortRenderableObjects()
	{
		_sortedRenderableObjects.clear();
		for (size_t i = 0; i != _renderableObjects.size(); ++i)
		{
			if (!_renderableObjects[i]->isVisible())
			{
				continue;
			}
			int minIndex = 0;
			if (i == 0)
			{
				_sortedRenderableObjects.push_back(0);
			}
			else
			{
				minIndex = getShortestRenderableObjects();
				if (minIndex >= 0)
				{
					_sortedRenderableObjects.push_back(minIndex);
				}
			}
		}
	}

	int SceneManager::getShortestRenderableObjects( )
	{
		int index = -1;
		int shortest = -1;
		size_t current_index = _sortedRenderableObjects[_sortedRenderableObjects.size() - 1];
		RenderSpace* rs_a = _renderableObjects[current_index]->getRenderSpaceVector();
		for(size_t i = 0; i != _renderableObjects.size(); ++i)
		{
			// self
			if (current_index == i)
			{
				continue;
			}
			// invisible
			if (!_renderableObjects[i]->isVisible())
			{
				continue;
			}
			// sorted
			std::vector<size_t>::iterator vt = std::find(_sortedRenderableObjects.begin(), _sortedRenderableObjects.end(), i);
			if (vt != _sortedRenderableObjects.end())
			{
				continue;
			}
			// to be processed
			RenderSpace* rs_b = _renderableObjects[i]->getRenderSpaceVector();
			std::vector<size_t>& ds = rs_a->difference(*rs_b);			
			if (shortest == -1)
			{
				index = i;
				shortest = ds.size();
			}
			else
			{
				if (ds.size() < shortest)
				{
					index = i;
					shortest = ds.size();
				}
			}
		}

		return index;
	}

	//const std::string SceneManager::_RootLightNode("RootLightNode");

	const std::string SceneManager::_RootSceneNode("RootSceneNode");

}
