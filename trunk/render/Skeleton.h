//==========================================================================
/**
* @file	  : Skeleton.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-25   21:06
* purpose : 
*/
//==========================================================================

#ifndef __Skeleton_h__
#define __Skeleton_h__

#include "Common.h"
#include "EntityYZ.h"

namespace Euclid
{
	  /** A collection of Bone objects used to animate a skinned mesh.
    @remarks
        Skeletal animation works by having a collection of 'bones' which are 
        actually just joints with a position and orientation, arranged in a tree structure.
        For example, the wrist joint is a child of the elbow joint, which in turn is a
        child of the shoulder joint. Rotating the shoulder automatically moves the elbow
        and wrist as well due to this hierarchy.
    @par
        So how does this animate a mesh? Well every vertex in a mesh is assigned to one or more
        bones which affects it's position when the bone is moved. If a vertex is assigned to 
        more than one bone, then weights must be assigned to determine how much each bone affects
        the vertex (actually a weight of 1.0 is used for single bone assignments). 
        Weighted vertex assignments are especially useful around the joints themselves
        to avoid 'pinching' of the mesh in this region. 
    @par
        Therefore by moving the skeleton using preset animations, we can animate the mesh. The
        advantage of using skeletal animation is that you store less animation data, especially
        as vertex counts increase. In addition, you are able to blend multiple animations together
        (e.g. walking and looking around, running and shooting) and provide smooth transitions
        between animations without incurring as much of an overhead as would be involved if you
        did this on the core vertex data.
    @par
        Skeleton definitions are loaded from datafiles, namely the .skeleton file format. They
        are loaded on demand, especially when referenced by a Mesh.
    */
	class Skeleton
	{
	public:
		 /** Creates a brand new Bone owned by this Skeleton. 
        @remarks
            This method creates an unattached new Bone for this skeleton and assigns it a 
            specific name.Unless this is to be a root bone (there may be more than one of 
			these), you must attach it to another Bone in the skeleton using addChild for it to be any use.
            For this reason you will likely be better off creating child bones using the
            Bone::createChild method instead, once you have created the root bone. 
        @param name The name to give to this new bone - must be unique within this skeleton. 
            Note that the way OGRE looks up bones is via a numeric handle, so if you name a
            Bone this way it will be given an automatic sequential handle. The name is just
            for your convenience, although it is recommended that you only use the handle to 
            retrieve the bone in performance-critical code.
        */
		virtual BoneNode* createBoneNode(const std::string& name);

		/** Returns the number of bones in this skeleton. */
		virtual u16 getNumBoneNodes(void) const;

		virtual bool setRootBoneNode(const std::string& name);

		  /** Gets the root bone of the skeleton: deprecated in favour of getRootBoneIterator. 
        @remarks
            The system derives the root bone the first time you ask for it. The root bone is the
            only bone in the skeleton which has no parent. The system locates it by taking the
            first bone in the list and going up the bone tree until there are no more parents,
            and saves this top bone as the root. If you are building the skeleton manually using
            createBone then you must ensure there is only one bone which is not a child of 
            another bone, otherwise your skeleton will not work properly. If you use createBone
            only once, and then use Bone::createChild from then on, then inherently the first
            bone you create will by default be the root.
        */
        virtual BoneNode* getRootBoneNode(void);

		/** Gets a bone by it's name. */
		virtual BoneNode* getBoneNode(const std::string& name);

		virtual bool      hasBoneNode(const std::string& name);

	public:
		void			  update(const AnimationTime& at);
		void			  reset();

	public:
		Skeleton();
		~Skeleton();

	public:
		bool create();
		void destroy();

	protected:
		typedef stdext::hash_map<std::string, BoneNode*> BoneNodeMap;
		BoneNodeMap		_boneNodes;
		std::string		_rootName;
	
	};
}

#endif // __Skeleton_h__
 
