#ifndef __Skeleton_h__
#define __Skeleton_h__

#include "Common.h"
#include "KeyFrames.h"
#include "base/ConstBaseIterator.h"
#include "BoneNode.h"
namespace Zen
{
	class BoneNode;
	typedef stdext::hash_map<std::string, BoneNode*> BoneNodeMap;
	typedef	ConstBaseIterator<BoneNodeMap>	BoneNodeMapIterator;
	class _ZenExport_ Skeleton
	{
	public:
		virtual BoneNode* createBoneNode(const std::string& name);

		/** Returns the number of bones in this skeleton. */
		virtual u16 getNumBoneNodes(void) const;

		//virtual bool setRootBoneNode(const std::string& name);

        //virtual BoneNode* getRootBoneNode(void);

		/** Gets a bone by it's name. */
		virtual BoneNode* getBoneNode(const std::string& name);

		virtual bool      hasBoneNode(const std::string& name);

	public:
		void create(const std::string& fileName);
		void destroy();
		void update(const AnimationTime& at, Skin& s);
		void reset();
		void render();
		BoneNodeMapIterator	getCommandMapIterator(void) const;
		
	public:
		Skeleton();
		~Skeleton();

	private:
		void _clear();
		void _relate();
		//void _addRootNode();
		BoneNode* _getBoneNode(int id);
	protected:
		BoneNodeMap		_boneNodes;
		char			_version;
		std::vector<std::string> _roots;
	public:
		std::vector<Mat4> _matrices;
		std::vector<Mat4> _matricesFull;
		friend class BoneNode;
	};
}

#endif // __Skeleton_h__
 
