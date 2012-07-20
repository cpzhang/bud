#include <cctype>
#include "Skeleton.h"
#include "BoneNode.h"
#include "MZ.h"
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
		_clear();
	}

	Skeleton::~Skeleton()
	{
		_clear();
	}

	void Skeleton::create(const std::string& fileName)
	{
		//
		std::ifstream f(fileName.c_str(), std::ios::binary);
		if (!f.good())
		{
			return;
		}

		u32 version = 0;
		//	[Tag Size Data]
		int t;
		int s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(int));
			f.read((char*)&s, sizeof(int));

			if (s <= 0)
			{
				continue;
			}

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;

			switch (t)
			{
			case 'MVER':
				{
					f.read((char*)&_version, sizeof(_version));
				}break;
			case 'MBON': // นว๗ภ
				{
					u32 nBones;
					f.read((char*)&nBones,sizeof(nBones));

					for (u32 i = 0;i < nBones;i++)
					{
						//
						Bone* bone = new Bone;
						
						//
						f.read((char*)&bone->id,sizeof(bone->id));

						u8 JointnameLen; 
						f.read((char*)&JointnameLen,sizeof(JointnameLen));

						char name[MODEL_NAME_NODE_SIZE+1];
						f.read((char*)name,JointnameLen);
						name[JointnameLen] = 0;
					
						//
						bone->name = name;

						//ascii2unicode(name, bone->name);

						int parent;
						f.read((char*)&parent,sizeof(parent));

						//
						bone->parent = parent;

						Mat4 initialMatrix = Mat4::IDENTITY;
						{
							f.read((char*)&initialMatrix, sizeof(initialMatrix));
						}
						bone->initialMatrix = initialMatrix;
						//
						BoneNode* node = createBoneNode(bone->name);
						//
						node->attachObject(bone);
					}
					if (nBones != 0)
					{
						_relate();
					}
					
				}break;
			default:
				{
					f.ignore(s);
				}break;
			}
		}
	}

	void Skeleton::destroy()
	{
		for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
		{
			BoneNode* r = it->second;
			if (r)
			{
				Bone* b = r->getBone();
				delete b;
				r->detachObject();
				delete r;
			}
		}
		
		_clear();
	}

	u16 Skeleton::getNumBoneNodes( void ) const
	{
		return _boneNodes.size();
	}

// 	BoneNode* Skeleton::getRootBoneNode( void )
// 	{
// 		BoneNodeMap::iterator it = _boneNodes.find(_rootName);
// 		if (it == _boneNodes.end())
// 		{
// 			return NULL;
// 		}
// 
// 		return it->second;
// 	}

	BoneNode* Skeleton::getBoneNode( const std::string& name )
	{
		if (_boneNodes.find(name) == _boneNodes.end())
		{
			return NULL;
		}

		return _boneNodes[name];
	}
// 
// 	bool Skeleton::setRootBoneNode( const std::string& name )
// 	{
// 		if (_boneNodes.find(name) == _boneNodes.end())
// 		{
// 			return false;
// 		}
// 
// 		_rootName = std::string(name);
// 
// 		return true;
// 	}

	bool Skeleton::hasBoneNode( const std::string& name )
	{
		if (_boneNodes.find(name) == _boneNodes.end())
		{
			return false;
		}

		return true;
	}

	void Skeleton::update( const AnimationTime& at, Skin& s )
	{
		for (size_t i = 0; i != _roots.size(); ++i)
		{
			BoneNodeMap::iterator it = _boneNodes.find(_roots[i]);
			if (it == _boneNodes.end())
			{
				return;
			}
			it->second->update(at, s);	
		}
	}

	void Skeleton::_clear()
	{
		_boneNodes.clear();
		_matrices.clear();
		_roots.clear();
	}

	BoneNode* Skeleton::_getBoneNode( int id )
	{
		for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
		{
			BoneNode* node = it->second;
			Bone* bone = node->getBone();
			if (id == bone->id)
			{
				return node;
			}
		}
		return NULL;
	}

	void Skeleton::_relate()
	{
		for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
		{
			BoneNode* node = it->second;
			if (node)
			{
				Bone* bone = node->getBone();
				if (bone)
				{
					int parentID = bone->parent;
					BoneNode* parentNode = _getBoneNode(parentID);
					if (parentNode)
					{
						parentNode->addChild(node);
					}
					else
					{
						_roots.push_back(bone->name);
					}
				}
			}
		}
		if (_roots.empty())
		{
			throw Buddha::Exception("no bone root found!");
		}
// 		if (roots.size() > 1)
// 		{
// 			for (size_t i = 0; i != roots.size(); ++i)
// 			{
// 				std::string name = roots[i];
// 				std::transform(name.begin(), name.end(), name.begin(), std::tolower);
// 				if (name == "root")
// 				{
// 					_rootName = roots[i];
// 					break;
// 				}
// 			}
// 			if (_rootName.empty())
// 			{
// 				_addRootNode();
// 				_relate();
// 			}
// 		}
// 		else
// 		{
// 			_rootName = roots[0];
// 		}
		
		//
		_matrices.resize(_boneNodes.size());
		_matricesFull.resize(_boneNodes.size());
	}

	Euclid::BoneNodeMapIterator Skeleton::getCommandMapIterator( void ) const
	{
		return BoneNodeMapIterator(_boneNodes.begin(), _boneNodes.end());
	}

// 	void Skeleton::_addRootNode()
// 	{
// 		//
// 		Bone* bone = new Bone;
// 
// 		//
// 		bone->name = "root";
// 
// 		//
// 		bone->parent = -2;
// 		bone->id = -1;
// 
// 		bone->initialMatrix = Mat4::IDENTITY;
// 		//
// 		BoneNode* nodeRoot = createBoneNode(bone->name);
// 		//
// 		nodeRoot->attachObject(bone);
// 		//
// 		for (BoneNodeMap::iterator it = _boneNodes.begin(); it != _boneNodes.end(); ++it)
// 		{
// 			BoneNode* node = it->second;
// 			if (node == nodeRoot)
// 			{
// 				continue;
// 			}
// 			if (node)
// 			{
// 				Bone* bone = node->getBone();
// 				if (bone)
// 				{
// 					int parentID = bone->parent;
// 					BoneNode* parentNode = _getBoneNode(parentID);
// 					if (NULL == parentNode)
// 					{
// 						bone->parent = -1;
// 					}
// 				}
// 			}
// 		}
// 	}

}
