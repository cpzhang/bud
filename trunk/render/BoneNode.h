#ifndef __BoneNode_h__
#define __BoneNode_h__

#include "Common.h"
#include "KeyFrames.h"
namespace Zen
{	
	struct sBoneKFs
	{
	public:
		sBoneKFs()
		{
			_clear();
		}
	private:
		void _clear()
		{
			translationKFs.setStaticData(Vec3(0.0f, 0.0f, 0.0f));
			rotationKFs.setStaticData(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
			scaleKFs.setStaticData(Vec3(1.0f, 1.0f, 1.0f));
			
			//
			translationKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
			rotationKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
			scaleKFs.setInterpolationType(eInterpolationType_Bezier_Linear);
		}
	public:
		int id;
		sKeyFrames<Quaternion>	rotationKFs;			/// Ðý×ª¹Ø¼üÖ¡
		sKeyFrames<Vec3>		scaleKFs;				/// Ëõ·Å¹Ø¼üÖ¡
		sKeyFrames<Vec3>		translationKFs;			/// Æ½ÒÆ¹Ø¼üÖ¡
	};

	struct Skin
	{
	public:
		Skin()
		{
			_clear();
		}

	public:
		void create(const std::string& fileName)
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
						f.read((char*)&mVersion, sizeof(mVersion));
					}break;
				case 'MBON': // ¹Ç÷À
					{
						Skin k;
						u32 numBones;
						f.read((char*)&numBones,sizeof(numBones));
						for (u32 i = 0;i < numBones;i++)
						{
							sBoneKFs s;
							u32 nKeyframes;
							// translate
							f.read((char*)&nKeyframes,sizeof(nKeyframes));
							for (u32 j = 0;j < nKeyframes;j++)
							{
								sKeyFrame<Vec3> kf;
								f.read((char*)&kf,sizeof(kf));
								s.translationKFs.addKeyFrame(kf);
							}
							// rotate
							f.read((char*)&nKeyframes,sizeof(nKeyframes));
							for (u32 j = 0;j < nKeyframes;j++)
							{
								sKeyFrame<Quaternion> kf;
								f.read((char*)&kf,sizeof(kf));
								s.rotationKFs.addKeyFrame(kf);
							}
							//scale
							f.read((char*)&nKeyframes,sizeof(nKeyframes));
							for (u32 j = 0;j < nKeyframes;j++)
							{
								sKeyFrame<Vec3> kf;
								f.read((char*)&kf,sizeof(kf));
								s.scaleKFs.addKeyFrame(kf);
							}
							//
							boneKFs.push_back(s);
						}
					}break;
				default:
					{
						f.ignore(s);
					}break;
				}
			}
		}
	private:
		void _clear()
		{
			boneKFs.clear();
		}
	public:
		char mVersion;
		std::vector<sBoneKFs> boneKFs;
	};
	class _ZenExport_ Bone
	{
	public:
		Bone()
		{
			_clear();
		}
	private:
		void _clear()
		{
			initialMatrix = Mat4::IDENTITY;
			id = 0;
			parent = 0;
			name.clear();
		}
	public:
		Mat4	initialMatrix;
		int		id;
		int     parent;
		std::string name;
	};

	class Skeleton;
	class _ZenExport_ BoneNode
	{
	public:
		BoneNode(Skeleton* ske, const std::string& name);
		~BoneNode();

	public:
		void	attachObject(Bone* bone);
		void	detachObject();
		void	update(const AnimationTime& at, Skin& s);
		void    addChild(BoneNode* node);
		Bone*	getBone()
		{
			return _bone;
		}
		BoneNode* getParentNode();
		typedef std::map<std::string, BoneNode*> NameNodeMap;
	private:
		void _clear();
	private:
		Bone*		_bone;

		/// Pointer back to creator, for child creation (not smart ptr so child does not preserve parent)
		Skeleton*	_skeleton;
		Mat4		_mtxTransform;
		Mat4		_fullMatrix;
		std::string _name;
		BoneNode* _parent;
		NameNodeMap _children;
	};
}


#endif // __BoneNode_h__
 
