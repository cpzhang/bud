//==========================================================================
/**
* @file	  : Node.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-5   22:00
* purpose : 
*/
//==========================================================================

#ifndef __Node_h__
#define __Node_h__

#include "Common.h"
#include "Appearance.h"
//
namespace Euclid
{
	class _EuclidExport_ Node
	{
	public:
		Node();
		Node(const std::string& name);
		virtual ~Node();

		//
	public:
		/** Enumeration denoting the spaces which a transform can be relative to.
        */
        enum TransformSpace
        {
            /// Transform is relative to the local space
            TS_LOCAL,
            /// Transform is relative to the space of the parent node
            TS_PARENT,
            /// Transform is relative to world space
            TS_WORLD
        };

		//
	public:
		void release();
		//
		const std::string& getName() const;

		//
		void	setParent(Node* parent);
		Node*	getParent() const;

		//
		virtual void render(eRenderQueueOrder order) = 0;

		//
		virtual Node*	createChild(const std::string& name) = 0;
		void	addChild(Node* child);
		void	eraseChild(const std::string& name);// just erase
		Node*	addChild(const std::string& name);
		Node*	deleteChild(const std::string& name);
		Node*	deleteChild(Node* child);
		void	deleteChildren(void);
		Node*	getChild(const std::string& name) const;
		size_t  getChildrenSize() const;

		//	Sets the position of the node relative to it's parent.
		void	setPosition(const Euler::Vector3& position);
		void	setPosition(Real x, Real y, Real z);
		const Euler::Vector3&		getPosition() const;
		
		//	Sets the scaling factor applied to this node.
		void	setScale(const Euler::Vector3& scale);
		void	setScale(Real x, Real y, Real z);
		void	setScale(Real r);
		const Euler::Vector3& getScale() const;
		
		//
        const Euler::Quaternion & getOrientation() const;
		void setOrientation(const Euler::Quaternion& q);
		void setOrientation( Real x, Real y, Real z, Real w );
		void resetQrientation();

		//
		void reset();
		//
	public:

		//	Scales the node, combining it's current scale with the passed in scaling factor.
		void scale(const Euler::Vector3& s);
		void scale(Real x, Real y, Real z);


		//	Moves the node along the Cartesian axes.
		void translate(const Euler::Vector3& d, TransformSpace relativeTo = TS_PARENT);
		void translate(Real x, Real y, Real z, TransformSpace relativeTo = TS_PARENT);
		void translate(const Euler::Matrix3& axes, const Euler::Vector3& move, TransformSpace relativeTo = TS_PARENT);
		void translate(const Euler::Matrix3& axes, Real x, Real y, Real z, TransformSpace relativeTo = TS_PARENT);

		//	Rotate the node around the Z-axis.
		void roll(const Euler::Radian& angle, TransformSpace relativeTo = TS_LOCAL);

		//	Rotate the node around the X-axis.
		void pitch(const Euler::Radian& angle, TransformSpace relativeTo = TS_LOCAL);

		//	Rotate the node around the Y-axis.
		void yaw(const Euler::Radian& angle, TransformSpace relativeTo = TS_LOCAL);

		//	Rotate the node around an arbitrary axis.
		void rotate(const Euler::Vector3& axis, const Euler::Radian& angle, TransformSpace relativeTo = TS_LOCAL);

		//	Rotate the node around an aritrary axis using a Quarternion.
		void rotate(const Euler::Quaternion& q, TransformSpace relativeTo = TS_LOCAL);

		/** Gets the full transformation matrix for this node.
            @remarks
                This method returns the full transformation matrix
                for this node, including the effect of any parent node
                transformations, provided they have been updated using the Node::_update method.
                This should only be called by a SceneManager which knows the
                derived transforms have been updated before calling this method.
                Applications using Ogre should just use the relative transforms.
        */
		virtual const Euler::Matrix4& _getFullTransform(void) const;


		//	Triggers the node to update it's combined transforms
		void _updateFromParent(void) const;

		 /** To be called in the event of transform changes to this node that require it's recalculation.
        @remarks
            This not only tags the node state as being 'dirty', it also requests it's parent to 
            know about it's dirtiness so it will get an update next time.
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
        */
        virtual void needUpdate(bool forceParentUpdate = false);

		/** Called by children to notify their parent that they need an update. 
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
		*/
        virtual void requestUpdate(Node* child, bool forceParentUpdate = false);

	protected:
		//	Gets the orientation of the node as derived from all parents.
		const Euler::Quaternion & _getDerivedOrientation(void) const;

		//	Gets the scaling factor of the node as derived from all parents.
		const Euler::Vector3 & _getDerivedScale(void) const;

		//	Gets the position of the node as derived from all parents.
		const Euler::Vector3 & _getDerivedPosition(void) const;

		//
	protected:
		friend class SceneManager;
		//
		Node*								_parent;

		//
		typedef		std::map<std::string, Node*>	NameNodeMap;
		NameNodeMap							_children;

		//
		std::string							_name;

		// Stores the position/translation of the node relative to its parent.
		Euler::Vector3						_position;

		// Stores the scaling factor applied to this node
		Euler::Vector3						_scale;

		// Stores the orientation of the node relative to it's parent.
		Euler::Quaternion					_orientation;

		/** Cached combined orientation.
            @par
                This member is the orientation derived by combining the
                local transformations and those of it's parents.
                This is updated when _updateFromParent is called by the
                SceneManager or the nodes parent.
        */
		mutable Euler::Quaternion			_derivedOrientation;

        /** Cached combined position.
            @par
                This member is the position derived by combining the
                local transformations and those of it's parents.
                This is updated when _updateFromParent is called by the
                SceneManager or the nodes parent.
        */
        mutable Euler::Vector3				_derivedPosition;

        /** Cached combined scale.
            @par
                This member is the position derived by combining the
                local transformations and those of it's parents.
                This is updated when _updateFromParent is called by the
                SceneManager or the nodes parent.
        */
        mutable Euler::Vector3				_derivedScale;

		/// Cached derived transform as a 4x4 matrix
		mutable Euler::Matrix4				_cachedTransform;

		/// Stores whether this node inherits orientation from it's parent
		bool _inheritOrientation;

		/// Stores whether this node inherits scale from it's parent
		bool _inheritScale;

		/// Flag to indicate own transform from parent is out of date
		mutable bool _needParentUpdate;
		/// Flag indicating that all children need to be updated
		mutable bool _needChildUpdate;
		/// Flag indicating that parent has been notified about update request
		mutable bool _parentNotified ;
		/// Flag indicating that the node has been queued for update
		mutable bool _queuedForUpdate;
		mutable bool _cachedTransformOutOfDate;

		typedef std::vector<Node*> ChildUpdateSet;
		/// List of children which need updating, used if self is not out of date but children are
		mutable ChildUpdateSet _childrenToUpdate;
	};
}

#endif // __Node_h__
 
