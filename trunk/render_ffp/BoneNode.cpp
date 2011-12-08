#include "BoneNode.h"
#include "Skeleton.h"
#include "D3D9Mapping.h"

namespace Euclid
{
	BoneNode::BoneNode( Skeleton* ske, const std::string& name )
		: _bone(NULL), _skeleton(ske)
	{
		_name = name;
	}
	BoneNode::~BoneNode()
	{

	}

	Node* BoneNode::createChild( const std::string& name )
	{
		BoneNode* b = NULL;

		b = _skeleton->createBoneNode(name);

		if (b != NULL)
		{
			this->addChild(b);
		}

		return b;
	}

	void BoneNode::attachObject( sBone* bone )
	{
		_bone = bone;
// 		if (_bone != NULL)
// 		{
// 			_inheritOrientation = !_bone->_notInheritRotation;
// 			_inheritScale = !_bone->_notInheritScaling;
// 		}
	}

	void BoneNode::detachObject()
	{
		_bone = NULL;
		
		NameNodeMap::iterator it;
		for (it = _children.begin(); it != _children.end(); ++it)
		{
			BoneNode* n = static_cast<BoneNode*>(it->second);
			n->detachObject();
			//n->deleteChildren();
		}
	}

	Quaternion convertQ(const Quaternion& q)
	{
		Quaternion qq;
		qq._x = -q._x;
		qq._y = -q._y;
		qq._z = -q._z;
		qq._w = q._w;

		return qq;
	}

	void BoneNode::update( const AnimationTime& at )
	{		
		//translation
		Vec3 t = _bone->_translation.getFrame(-1, &at);
		//rotation
		Quaternion q = _bone->_rotation.getFrame(-1, &at);
		//scale
		Vec3 s = _bone->_scale.getFrame(-1, &at);

		Mat4 dynamicMtx = Mat4::IDENTITY;
		dynamicMtx.makeTransform(t, s, q);

		//
		Node* parentNode = _parent;
		if (parentNode)
		{
			BoneNode* p = static_cast<BoneNode*>(parentNode);
			_mtxTransform =  p->_fullMatrix * dynamicMtx;
		}
		else
		{
			_mtxTransform = dynamicMtx;
		}
		_fullMatrix = _mtxTransform;
		_bone->_pivotPointTransformed = _fullMatrix * Vec3::ZERO;
		//
		_mtxTransform = _fullMatrix * _bone->_initialMatrix.inverse();

		_bone->_finalMtx = _mtxTransform;
		
		NameNodeMap::iterator it = _children.begin();
		for ( ; it != _children.end(); ++it)
		{
			BoneNode* n = static_cast<BoneNode*>(it->second);
			n->update(at);
		}
	}
}
