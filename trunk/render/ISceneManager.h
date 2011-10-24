//==========================================================================
/**
* @file	  : ISceneManager.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-7   20:47
* purpose : 
*/
//==========================================================================

#ifndef __ISceneManager_h__
#define __ISceneManager_h__

#include "Common.h"
#include "Appearance.h"
//
namespace Euclid
{
	class ISceneManager
	{
	public:
		virtual EntityYZ* createEntityYZ(EntityYZPara* p) = 0;

		virtual EntityYZInstance* createEntityYZInstance(EntityYZInstancePara* p) = 0;

		virtual ParticleSystem* createParticleSystem(ParticleSystemPara* para) = 0;
		//
		virtual Billboard* createBillboard(BillboardPara* para) = 0;

		//
		virtual EntityX* createEntityX(const std::string& EntityXName, const std::string& meshName) = 0;

		//
		virtual EntityMD2* createEntityMD2(const std::string& EntityMD2Name, const std::string& md2Name) = 0;

		//
		virtual ManualObject* createManualObject(const std::string& name) = 0;

		//
		virtual ManualObject* createManualObject(ManualPara& para) = 0;

		//
		virtual Background* createBackground(BackgroundPara& para) = 0;

		//
		virtual SkyBox* createSkyBox(const std::string& name, const std::string& file) = 0;

		//
		virtual SceneNode* getRootSceneNode() = 0;

		//
		virtual SceneNode* createSceneNode(const std::string& name) = 0;

		//
		virtual void destroySceneNodeAndChildren(const std::string& name) = 0;

		virtual void destroyAllSceneNodes() = 0;

		//
		virtual SceneNode* getSceneNode(const std::string& name) = 0;

		//
		virtual void render(eRenderQueueOrder order = eRenderQueueOrder_Size) = 0;

		//
		virtual Light* createLight(const std::string& name) = 0;

		virtual void update(u32 current, u32 delta) = 0;

		////
		//virtual LightNode* getRootLightNode() = 0;

		////
		//virtual LightNode* createLightNode(const std::string& name) = 0;
		//
		virtual void onAttachObject(RenderableObject* o) = 0;
		virtual void onDettachObject(RenderableObject* o) = 0;
	};
}

#endif // __ISceneManager_h__
 
