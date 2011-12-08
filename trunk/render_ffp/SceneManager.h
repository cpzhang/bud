//==========================================================================
/**
* @file	  : SceneManager.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   19:45
* purpose : 
*/
//==========================================================================

#ifndef __SceneManager_h__
#define __SceneManager_h__

#include "Common.h"
#include "ISceneManager.h"

//
namespace Euclid
{
	class SceneManager : public ISceneManager, public Buddha::SingletonEx<SceneManager>
	{
	public:
		SceneManager();
		~SceneManager();

		//
	public:
		bool create();

		void destroy();

		virtual EntityYZ* createEntityYZ(EntityYZPara* p);

		virtual EntityYZInstance* createEntityYZInstance(EntityYZInstancePara* p);

		virtual ParticleSystem* createParticleSystem(ParticleSystemPara* para);

		virtual Billboard* createBillboard(BillboardPara* para);

		//	Create an EntityX
		virtual EntityX* createEntityX(const std::string& EntityXName, const std::string& meshName);

		//
		virtual EntityMD2* createEntityMD2(const std::string& EntityMD2Name, const std::string& md2Name);

		//
		virtual ManualObject* createManualObject(const std::string& name);

		virtual ManualObject* createManualObject(ManualPara& para);

		virtual Background* createBackground(BackgroundPara& para);

		//
		virtual SkyBox* createSkyBox(const std::string& name, const std::string& file);

		//
		virtual SceneNode* getRootSceneNode();

		//
		virtual SceneNode* createSceneNode(const std::string& name);

		virtual void destroySceneNodeAndChildren(const std::string& name);
		virtual void _destroySceneNodeAndChildren(const std::string& name);
		virtual void destroyAllSceneNodes();
		//
		virtual SceneNode* getSceneNode(const std::string& name);

		//
		virtual void render(eRenderQueueOrder order = eRenderQueueOrder_Size);

		//
		virtual Light* createLight(const std::string& name);

		virtual void update(u32 current, u32 delta);
		////
		//virtual LightNode* getRootLightNode();

		////
		//virtual LightNode* createLightNode(const std::string& name);
	
		virtual void onAttachObject(RenderableObject* o);
		//
		virtual void onDettachObject(RenderableObject* o);
		//
		void sortRenderableObjects();
		//
		int getShortestRenderableObjects();
		//
	protected:
		typedef stdext::hash_map<std::string, SceneNode*> SceneNodeMap;
		//typedef stdext::hash_map<std::string, LightNode*> LightNodeMap;
		SceneNodeMap				_nodes;
		//LightNodeMap				_lights;
		static const std::string	_RootSceneNode;
		//static const std::string	_RootLightNode;
		//
		std::vector<RenderableObject*>	_renderableObjects;
		std::vector<size_t>				_sortedRenderableObjects;
	};
}

#endif // __SceneManager_h__
 
