#include "stdafx.h"
#include "Node.h"

//
namespace Euclid
{
	Node::Node()
		:	_parent(0), _orientation(Euler::Quaternion::IDENTITY), _position(Euler::Vector3::ZERO),
		_scale(Euler::Vector3::UNIT_SCALE), _derivedOrientation(Euler::Quaternion::IDENTITY), 
		_derivedPosition(Euler::Vector3::ZERO), _derivedScale(Euler::Vector3::UNIT_SCALE)
	{

	}

	Node::Node( const std::string& name )
		:	_parent(NULL), _orientation(Euler::Quaternion::IDENTITY), _position(Euler::Vector3::ZERO),
		_scale(Euler::Vector3::UNIT_SCALE), _derivedOrientation(Euler::Quaternion::IDENTITY), 
		_derivedPosition(Euler::Vector3::ZERO), _derivedScale(Euler::Vector3::UNIT_SCALE),
		_name(name), _inheritScale(true), _inheritOrientation(true), _needParentUpdate(false), _needChildUpdate(false),
		_parentNotified(false), _queuedForUpdate(false), _cachedTransformOutOfDate(true)
	{
		 needUpdate();
	}
	Node::~Node()
	{
		//
		//deleteChildren();

		//
		//if (_parent)
		//{
		//	_parent->deleteChild(this);
		//}

		//if (_queuedForUpdate)
		//{
		//	// Erase from queued updates
		//	QueuedUpdates::iterator it =
		//		std::find(msQueuedUpdates.begin(), msQueuedUpdates.end(), this);
		//	assert(it != msQueuedUpdates.end());
		//	if (it != msQueuedUpdates.end())
		//	{
		//		// Optimised algorithm to erase an element from unordered vector.
		//		*it = msQueuedUpdates.back();
		//		msQueuedUpdates.pop_back();
		//	}
		//}

	}

	const std::string& Node::getName() const
	{
		return _name;
	}


	void Node::setParent( Node* parent )
	{
		_parent = parent;

		// Request update from parent
		_parentNotified = false ;
		if (_parent)
		{
			needUpdate();
		}
	}

	Node* Node::getParent() const
	{
		return _parent;
	}

	void Node::setPosition( const Euler::Vector3& position )
	{
		_position = position;
		  needUpdate();
	}

	void Node::setPosition( Real x, Real y, Real z )
	{
		setPosition(Euler::Vector3(x, y, z));
	}
	const Euler::Vector3& Node::getPosition() const
	{
		return _position;
	}

	void Node::setScale( const Euler::Vector3& scale )
	{
		_scale = scale;
		  needUpdate();
	}

	void Node::setScale( Real x, Real y, Real z )
	{
		setScale(Euler::Vector3(x, y, z));
	}

	void Node::setScale( Real r )
	{
		setScale(Euler::Vector3(r, r, r));
	}

	const Euler::Vector3& Node::getScale() const
	{
		return _scale;
	}

	void Node::addChild( Node* child )
	{
		_children[child->_name] = child;
		child->_parent = this;
	}

	Node* Node::addChild( const std::string& name )
	{
		Node* n = createChild(name);
		n->_parent = this;
		_children[name] = n;
		
		//
		return n;
	}

	Node* Node::getChild( const std::string& name ) const
	{
		NameNodeMap::const_iterator it = _children.find(name);
		return it->second;
	}

	Node* Node::deleteChild( const std::string& name )
	{
		NameNodeMap::iterator it = _children.find(name);
		
		//
		Node* n = it->second;

		//
		_children.erase(it);

		//
		return n;
	}

	Node* Node::deleteChild( Node* child )
	{
		if (NULL == child)
		{
			return NULL;
		}

		//
		NameNodeMap::iterator it = _children.find(child->_name);

		//
		Node* n = it->second;

		//
		_children.erase(it);

		//
		return n;
	}

	void Node::deleteChildren( void )
	{
		NameNodeMap::iterator i, iend;
		iend = _children.end();
		for (i = _children.begin(); i != iend; ++i)
		{
			i->second->setParent(NULL);
		}
		_children.clear();
	}

	const Euler::Quaternion & Node::getOrientation() const
	{
		return _orientation;
	}

	void Node::setOrientation( const Euler::Quaternion& q )
	{
		_orientation = q;
		_orientation.normalize();
		 needUpdate();
	}

	void Node::setOrientation( Real x, Real y, Real z, Real w )
	{
		setOrientation(Euler::Quaternion(x, y, z, w));
	}

	void Node::resetQrientation()
	{
		_orientation = Euler::Quaternion::IDENTITY;
		 needUpdate();
	}

	void Node::scale( const Euler::Vector3& s )
	{
		_scale *= s;
		 needUpdate();
	}

	void Node::scale( Real x, Real y, Real z )
	{
		scale(Euler::Vector3(x, y, z));
	}

	void Node::translate( const Euler::Vector3& d, TransformSpace relativeTo /*= TS_PARENT*/ )
	{
		switch (relativeTo)
		{
		case TS_LOCAL:
			_position = _orientation * d; 
			break;
		case TS_PARENT:
			_position += d;
			break;
		case TS_WORLD:
			if (_parent)
			{
				_position += (_parent->_getDerivedOrientation().inverse() * d) / _parent->_getDerivedScale();
			}
			else
			{
				_position += d;
			}
			break;
		}
		needUpdate();
	}

	void Node::translate( Real x, Real y, Real z, TransformSpace relativeTo /*= TS_PARENT*/ )
	{
		translate(Euler::Vector3(x, y, z), relativeTo);
	}

	void Node::translate( const Euler::Matrix3& axes, const Euler::Vector3& move, TransformSpace relativeTo /*= TS_PARENT*/ )
	{
		Euler::Vector3 derived = axes * move;
		translate(derived, relativeTo);
	}

	void Node::translate( const Euler::Matrix3& axes, Real x, Real y, Real z, TransformSpace relativeTo /*= TS_PARENT*/ )
	{
		translate(axes, Euler::Vector3(x, y, z), relativeTo);
	}
	const Euler::Quaternion & Node::_getDerivedOrientation( void ) const
	{
		if (_needParentUpdate)
		{
			_updateFromParent();
		}

		return _derivedOrientation;
	}

	void Node::_updateFromParent( void ) const
	{
		if (_parent)
		{
			const Euler::Quaternion& p = _parent->_getDerivedOrientation();
			if (_inheritOrientation)
			{
				_derivedOrientation = p * _orientation;
			}
			else
			{
				_derivedOrientation = _orientation;
			}
			
			//
			const Euler::Vector3& s = _parent->_getDerivedScale();
			if (_inheritScale)
			{
				_derivedScale = s * _scale;
			}
			else
			{
				_derivedScale = _scale;
			}
			
			//
			_derivedPosition = p * (s * _position);
			_derivedPosition += _parent->_getDerivedPosition();
		}
		else
		{
			//	Root node, no parent
			_derivedOrientation = _orientation;
			_derivedPosition = _position;
			_derivedScale = _scale;
		}

		_cachedTransformOutOfDate = true;
		_needParentUpdate = false;
	}

	const Euler::Vector3 & Node::_getDerivedScale( void ) const
	{
		if (_needParentUpdate)
		{
			_updateFromParent();
		}

		return _derivedScale;
	}

	const Euler::Vector3 & Node::_getDerivedPosition( void ) const
	{
		if (_needParentUpdate)
		{
			_updateFromParent();
		}
		
		return _derivedPosition;
	}

	void Node::roll( const Euler::Radian& angle, TransformSpace relativeTo /*= TS_LOCAL*/ )
	{
		rotate(Euler::Vector3::UNIT_Z, angle, relativeTo);
	}

	void Node::pitch( const Euler::Radian& angle, TransformSpace relativeTo /*= TS_LOCAL*/ )
	{
		rotate(Euler::Vector3::UNIT_X, angle, relativeTo);
	}

	void Node::yaw( const Euler::Radian& angle, TransformSpace relativeTo /*= TS_LOCAL*/ )
	{
		rotate(Euler::Vector3::UNIT_Y, angle, relativeTo);
	}

	void Node::rotate( const Euler::Vector3& axis, const Euler::Radian& angle, TransformSpace relativeTo /*= TS_LOCAL*/ )
	{
		Euler::Quaternion q;
		q.FromAngleAxis(angle,axis);
		rotate(q, relativeTo);
	}

	void Node::rotate( const Euler::Quaternion& q, TransformSpace relativeTo /*= TS_LOCAL*/ )
	{
		// Normalise quaternion to avoid drift
		Euler::Quaternion qnorm = q;
		qnorm.normalize();

		switch(relativeTo)
		{
		case TS_PARENT:
			// Rotations are normally relative to local axes, transform up
			_orientation = qnorm * _orientation;
			break;
		case TS_WORLD:
			// Rotations are normally relative to local axes, transform up
			_orientation = _orientation * _getDerivedOrientation().inverse()
				* qnorm * _getDerivedOrientation();
			break;
		case TS_LOCAL:
			// Note the order of the mult, i.e. q comes after
			_orientation = _orientation * qnorm;
			break;
		}

		needUpdate();
	}

	const Euler::Matrix4& Node::_getFullTransform( void ) const
	{
		if (_cachedTransformOutOfDate)
		{
			// Use derived values
			_cachedTransform.makeTransform(_getDerivedPosition(), _getDerivedScale(), _getDerivedOrientation());
			_cachedTransformOutOfDate = false;
		}

		//
		return _cachedTransform;
	}

	void Node::reset()
	{
		_orientation = Euler::Quaternion::IDENTITY;
		_position = Euler::Vector3::ZERO;
		_scale = Euler::Vector3::UNIT_SCALE;
		_derivedOrientation = Euler::Quaternion::IDENTITY;
		_derivedPosition = Euler::Vector3::ZERO;
		_derivedScale = Euler::Vector3::UNIT_SCALE;

		NameNodeMap::iterator it = _children.begin();
		for ( ; it != _children.end(); ++it)
		{
			it->second->reset();
		}
		needUpdate();
	}

	void Node::needUpdate( bool forceParentUpdate /*= false*/ )
	{
		_needParentUpdate = true;
		_needChildUpdate = true;
		_cachedTransformOutOfDate = true;

		// Make sure we're not root and parent hasn't been notified before
		if (_parent && (!_parentNotified || forceParentUpdate))
		{
			_parent->requestUpdate(this, forceParentUpdate);
			_parentNotified = true ;
		}

		// all children will be updated
		_childrenToUpdate.clear();
	}

	void Node::requestUpdate( Node* child, bool forceParentUpdate /*= false*/ )
	{
		// If we're already going to update everything this doesn't matter
		if (_needChildUpdate)
		{
			return;
		}

		_childrenToUpdate.push_back(child);
		// Request selective update of me, if we didn't do it before
		if (_parent && (!_parentNotified || forceParentUpdate))
		{
			_parent->requestUpdate(this, forceParentUpdate);
			_parentNotified = true ;
		}
	}

	void Node::release()
	{
// 		NameNodeMap::iterator it;
// 		for (it = _children.begin(); it != _children.end(); ++it)
// 		{
// 			Node* n = it->second;
// 			if (n)
// 			{
// 				n->release();
// 				n = NULL;
// 			}
// 		}
//		_parent->deleteChild(this);
		_children.clear();

		delete this;
	}

	size_t Node::getChildrenSize() const
	{
		return _children.size();
	}

	void Node::eraseChild( const std::string& name )
	{
		NameNodeMap::iterator it = _children.find(name);
		if (it != _children.end())
		{
			_children.erase(it);
		}
	}

}
