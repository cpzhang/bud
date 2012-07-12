#ifndef __MZ_h__
#define __MZ_h__
#include "Common.h"
#include "Terrain.h"
#include "BoneNode.h"
#include "Color.h"
namespace Euclid
{
	class Buddha::MemoryFileWriter;
	template<class T>
	void writeSequence(Buddha::ChunkSet& cs, T &t)
	{
		T::iterator i = t.begin();
		for(; i != t.end(); ++i)
		{
			cs.write(&(*i),sizeof(*i));
		}
	}

	class _EuclidExport_ MZ
	{
	public:
		MZ();
		~MZ();

		static bool load(const std::string& fileName);
		static void loadMzI(const std::string& fileName);
		static void loadFb(const std::string& fileName);

		static Mesh* create(size_t sub, Mesh* m);
		static void destroy();
		static size_t getSubMeshNumber();
		static std::string getSubMeshName(size_t sub);
		static void clear();
		static void decodeVersion(std::ifstream& f, int s);
		static void decodeSubMesh(std::ifstream& f, int s);
		static void decodeVertices(std::ifstream& f, int s);
		static void decodeFaces(std::ifstream& f, int s);
		static void decodeMaterial(std::ifstream& f, int s);
		static void decodeAnimation(std::ifstream& f, int s);
		static void decodeBone(std::ifstream& f, int s);
		static void decodeRibbon(std::ifstream& f, int s);
		static void decodeParticle(std::ifstream& f, int s);
		//
		static void saveAnimation(const std::string& fileName);
		static void saveSkeleton(const std::string& fileName);
		static void saveSkin(const std::string& fileName);
		static void saveMaterial(const std::string& fileName);
		static void saveSubEntity(const std::string& fileName);
		static void saveEntity(const std::string& fileName);
	public:
		struct sSubMesh
		{
			sSubMesh()
			{
				memset(this, 0, sizeof(*this));
			}
			unsigned short matId;			//材质id
			unsigned short vstart;			// first vertex
			unsigned short vcount;			// num vertices
			unsigned short istart;			// first index
			unsigned short icount;			// num indices
			bool		animated;
			u8		nameLen;
		};

		struct sSubMeshWithName: public sSubMesh
		{
			sSubMeshWithName()
			{
				memset(this, 0, sizeof(*this));
			}
			char name[128];
		};

		struct sSubMeshWithName_V3
		{
			sSubMeshWithName_V3()
			{
				memset(this, 0, sizeof(*this));
			}
			unsigned short matId;			//材质id
			unsigned short vstart;			// first vertex
			unsigned short vcount;			// num vertices
			unsigned short istart;			// first index
			unsigned short icount;			// num indices
			bool		animated;
			char name[256];
		};

		struct sVertex_V3 
		{
			float pos[3];			/// 位置
			float normal[3];		/// 法线
			float color[4];			/// 颜色
			float weights[4];		/// 权重
			u8	bones[4];			/// 骨骼
			float texcoords[2];		/// 纹理坐标 
		};
		
		struct sVertex 
		{
			sVertex()
			{
				memset(this, 0, sizeof(*this));	
			}
			sVertex(const sVertex_V3& v)
			{
				memcpy(pos, v.pos, sizeof(pos));
				memcpy(normal, v.normal, sizeof(normal));
				memcpy(weights, v.weights, sizeof(weights));
				memcpy(bones, v.bones, sizeof(bones));
				memcpy(texcoords, v.texcoords, sizeof(texcoords));
			}
			
			float pos[3];
			float normal[3];
			float weights[4];
			u8 bones[4];
			float texcoords[2];
		};

		struct sVertex_V3Color:public sVertex
		{
			float color[4];
			sVertex_V3Color()
			{
				memset(this, 0, sizeof(*this));	
			}
			sVertex_V3Color(const sVertex_V3Color& v)
			{
				memcpy(this, &v, sizeof(*this));
			}
			sVertex_V3Color(const sVertex& v)
			{
				memcpy(pos, v.pos, sizeof(pos));
				memcpy(normal, v.normal, sizeof(normal));
				memcpy(weights, v.weights, sizeof(weights));
				memcpy(bones, v.bones, sizeof(bones));
				memcpy(texcoords, v.texcoords, sizeof(texcoords));
			}
			sVertex_V3Color(const sVertex_V3& v)
			{
				memcpy(pos, v.pos, sizeof(pos));
				memcpy(normal, v.normal, sizeof(normal));
				memcpy(weights, v.weights, sizeof(weights));
				memcpy(bones, v.bones, sizeof(bones));
				memcpy(texcoords, v.texcoords, sizeof(texcoords));
				memcpy(color, v.color, sizeof(color));
			}
		};
		struct sFace_V3
		{
			int index[3];
		};
		struct sMat
		{
			std::string mName;
			std::string mTextureName;
			eCullMode mCullMode;
			sKeyFrames<Quaternion>	mRotationKFs;			/// 旋转关键帧
			sKeyFrames<Vec3>		mScaleKFs;				/// 缩放关键帧
			sKeyFrames<Vec3>		mTranslationKFs;		/// 平移关键帧
			sKeyFrames<Color3>		mColorKFs;		/// 颜色关键帧
			sKeyFrames<float>		mAlphaKFs;		/// 透明度关键帧
			bool mRotate;
			bool mScale;
			bool mTranslate;
			eZBufferType mZEnable;
			bool mZWriteEnable;
			bool mAlphaTestEnable;
			eBlend mSrcBlend;
			eBlend mDestBlend;
			eCmpFunc mCmpFunc;
			u32 mAlphaRef;
			Color mDiffuse;
		};
		struct tranMatrix
		{
			float m_mat[4][3];
		};
		struct sAnimation
		{
			std::string name;
			u32 begin;
			u32 end;
		};
	private:
		//
		static char mVersion;
		typedef std::vector<sVertex_V3Color> VertexVec;
		static VertexVec	mVertices;
		typedef std::vector<sSubMeshWithName> SubMeshVec;
		static SubMeshVec	mSubmeshes;
		typedef std::vector<Mesh::sFace> FaceVec;
		static FaceVec	mFaces;
		typedef std::vector<sMat> MatVec;
		static MatVec	mMaterials;
		typedef std::vector<sAnimation> AniVec;
		static AniVec	mAnimations;
		static std::vector<std::string> mFbsFileNames;
		static std::vector<std::string> mAnimationNames;
		static std::vector<Bone> mBones;
		static std::vector<Skin> mSkins;
		static std::string mFilePath;
		static bool mLoadFBs;
		static bool mUseVertexColor;
	private:
		friend class Mesh;
	};
}

#endif //__MZ_h__