//==========================================================================
/**
* @file	  : BoneNode.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-25   20:56
* purpose : 
*/
//==========================================================================

#ifndef __BoneNode_h__
#define __BoneNode_h__

#include "Common.h"
#include "Node.h"
#include "EntityYZ.h"

namespace Euclid
{
	/** A bone in a skeleton.
    @remarks
        See Skeleton for more information about the principles behind skeletal animation.
        This class is a node in the joint hierarchy. Mesh vertices also have assignments
        to bones to define how they move in relation to the skeleton.
    */
	
	class BoneNode : public Node
	{
	public:
		BoneNode(Skeleton* ske, const std::string& name);
		~BoneNode();
	public:
		//
		virtual void render(eRenderQueueOrder order){};
		virtual Node*	createChild(const std::string& name);

	public:
		void	attachObject(sBone* bone);

		void	detachObject();

		void	update(const AnimationTime& at);

	protected:
		sBone*		_bone;

		/// Pointer back to creator, for child creation (not smart ptr so child does not preserve parent)
		Skeleton*	_skeleton;

		Mat4		_mtxTransform;
		Mat4		_fullMatrix;
	};
}


#endif // __BoneNode_h__
 
