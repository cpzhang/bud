#include "stdafx.h"
#include "Skeleton.h"
#include "BoneNode.h"

namespace Euclid
{
	BoneNode* Skeleton::createBoneNode( const std::string& name )
	{
		BoneNode* b = new BoneNode(this, name);
		
		if (b != NULL)
		{
			_boneNodes[name] = b;
		}

		return b;
	}

	Skeleton::Skeleton()
	{

	}

	Skeleton::~Skeleton()
	{
		//destroy();
	}

	bool Skeleton::create()
	{
		return true;
	}

	void Skeleton::destroy()
	{
		//STLDeleteAssociate(_boneNodes);
// 		BoneNodeMap::iterator it = _boneNodes.begin();
// 		for (; it != _boneNodes.end(); ++it)
// 		{
// 			BoneNode* n = it->second;
// 			if (n)
// 			{
// 				delete n;
// 				n = NULL;
// 			}
// 		}
		BoneNode* r = getRootBoneNode();
		if (r)
		{
			r->detachObject();
			r->release();
			//r = NULL;
		}
		_boneNodes.clear();
	}

	u16 Skeleton::getNumBoneNodes( void ) const
	{
		return _boneNodes.size();
	}

	BoneNode* Skeleton::getRootBoneNode( void )
	{
		BoneNodeMap::iterator it = _boneNodes.find(_rootName);
		if (it == _boneNodes.end())
		{
			return NULL;
		}

		return it->second;
	}

	BoneNode* Skeleton::getBoneNode( const std::string& name )
	{
		if (_boneNodes.find(name) == _boneNodes.end())
		{
			return NULL;
		}

		return _boneNodes[name];
	}

	bool Skeleton::setRootBoneNode( const std::string& name )
	{
		if (_boneNodes.find(name) == _boneNodes.end())
		{
			return false;
		}

		_rootName = std::string(name);

		return true;
	}

	bool Skeleton::hasBoneNode( const std::string& name )
	{
		if (_boneNodes.find(name) == _boneNodes.end())
		{
			return false;
		}

		return true;
	}

	void Skeleton::update( const AnimationTime& at )
	{
		BoneNodeMap::iterator it = _boneNodes.find(_rootName);
		if (it == _boneNodes.end())
		{
			return;
		}

		it->second->update(at);	
	}

	void Skeleton::reset()
	{
		BoneNodeMap::iterator it = _boneNodes.find(_rootName);
		if (it == _boneNodes.end())
		{
			return;
		}

		it->second->reset();;	
	}
}
