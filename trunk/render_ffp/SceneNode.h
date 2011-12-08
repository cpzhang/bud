//==========================================================================
/**
* @file	  : SceneNode.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-8   11:37
* purpose : 
*/
//==========================================================================

#ifndef __SceneNode_h__
#define __SceneNode_h__

#include "Common.h"
#include "Node.h"
#include "RenderableObject.h"

//
namespace Euclid
{
	/** Class representing a node in the scene graph.
        @remarks
            A SceneNode is a type of Node which is used to organise objects in a scene.
            It has the same hierarchical transformation properties of the generic Node class,
            but also adds the ability to attach world objects to the node, and stores hierarchical
            bounding volumes of the nodes in the tree.
            Child nodes are contained within the bounds of the parent, and so on down the
            tree, allowing for fast culling.
    */
	class _EuclidExport_ SceneNode : public Node
	{
	public:
		SceneNode(ISceneManager* sceneManager, const std::string& name);
		~SceneNode();

	public:
		//
		virtual void render(eRenderQueueOrder order);
		//
		Node*	createChild(const std::string& name);
	public:
		//
		void update(u32 current, u32 delta);
		//
		void attachObject(RenderableObject* object);
		RenderableObject*  getAttachedObject(const std::string& name);

		//
		void detachObject( const std::string& name);

		//
		void detachAllObjects();

		//
		SceneNode* addSceneNodeChild(const std::string& name);

		Mat4 getWorldViewProjectMatrix();
		//
	protected:
		ISceneManager*		_sceneManager;
		//
		typedef stdext::hash_map<std::string, RenderableObject*> AttachedObjectsMap;
		AttachedObjectsMap	_attachedObjects;
	};
}

#endif // __SceneNode_h__
 
