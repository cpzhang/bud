//==========================================================================
/**
* @file	  : EntityYZ.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-22   11:35
* purpose : 
*/
//==========================================================================

#ifndef __EntityYZ_h__
#define __EntityYZ_h__

#include "Common.h"
#include "RenderableObject.h"
#include "Color.h"
#include "KeyFrames.h"
#include "ParticleSystem2.h"
//#include "SubEntityYZ.h"

namespace Euclid
{
// #	pragma pack( push)
// #	pragma pack( 1 )

	class _EuclidExport_ EntityYZPara : public RenderablePara
	{
	public:
		EntityYZPara()
			:	_name(""), _path("")
		{
			_relativePath = true;
		}
		
		EntityYZPara(EntityYZPara* p)
		{
			if (p != NULL)
			{
				_name = p->_name;
				_path = p->_path;
				_relativePath = p->_relativePath;
			}
		}

	public:
		std::string		_name;
		std::string		_path;
		bool			_relativePath;
	};

	/// 子网格信息
	struct sSubMeshOld
	{
	public:
		sSubMeshOld()
		{
			memset(name, 0, 257);
		}
	public:
		u16		matId;			/// 材质ID
		u16		vstart;			/// 第一个顶点位置
		u16		vcount;			/// 顶点数目
		u16		istart;			/// 第一个索引位置
		u16		icount;			/// 索引数目
		bool	animated;		/// 是否有动画
		char	name[257];		/// 子网格名称
	};

	struct SUBMESH_BASE
	{
	public:
		SUBMESH_BASE()
			:	matId(0), vstart(0), vcount(0), istart(0), icount(0), animated(false), nameLen(0)
		{

		}
		SUBMESH_BASE(sSubMeshOld* b)
		{
			matId = b->matId;
			vstart = b->vstart;
			vcount = b->vcount;
			istart = b->istart;
			icount = b->icount;
			animated = b->animated;
			nameLen = 0;
		}
	public:
		u16		matId;			/// 材质ID
		u16		vstart;			/// 第一个顶点位置
		u16		vcount;			/// 顶点数目
		u16		istart;			/// 第一个索引位置
		u16		icount;			/// 索引数目
		bool	animated;		/// 是否有动画
		u8		nameLen;		/// 名称
	};

	struct sSubMesh
	{
	public:
		sSubMesh(sSubMeshOld* b)
		{
			_base = new SUBMESH_BASE(b);
			_name = b->name;
		}
		sSubMesh(SUBMESH_BASE* base, std::string name)
			:	_base(base), _name(name)
		{

		}
		~sSubMesh()
		{
			destroy();
		}
	public:
		inline void destroy()
		{
			safeDelete(_base);
		}
		
	public:
		SUBMESH_BASE*	_base;
		std::string		_name;
	};

	typedef std::map<std::string, sSubMesh*> SubMeshMap;
	
	struct sAnimation
	{
	public:
		sAnimation(std::string name, u32 start, u32 end)
			:	_name(name), _time_start(start), _time_end(end)
		{

		}
	public:
		std::string		_name;
		u32				_time_start;
		u32				_time_end;
	};

	typedef std::map<std::string, sAnimation*> AnimationMap;

	struct sVertexOld
	{
		Vec3	pos;
		Vec3	normal;
		Color	color;
		Vec4	weights;
		u8		bones[4];
		Vec2	texcoords;
	};
	
	struct sVertex
	{
		Vec3	pos;
		Vec3	normal;
		Vec4	weights;
		u8		bones[4];
		Vec2	texcoords;
	};

	struct sVertexEx
	{
	public:
		sVertexEx()
		{

		}
		sVertexEx(sVertex* v)
		{
			pos = v->pos;
			normal = v->normal;
			weights = v->weights;
			memcpy(bones, v->bones, 4);
			texcoords = v->texcoords;
			color = Color::White;
		}
		sVertexEx(sVertexOld* v)
		{
			pos = v->pos;
			normal = v->normal;
			weights = v->weights;
			memcpy(bones, v->bones, 4);
			texcoords = v->texcoords;
			color = v->color;
		}
	public:
		Vec3	pos;
		Vec3	normal;
		Vec4	weights;
		u8		bones[4];
		Vec2	texcoords;
		Color	color;
	};

	struct sFace
	{
		s16		index[3];
	};

	struct sTranMatrix
	{
		Real	_mat[4][3];
	};

	/// 关键帧平移参数
	struct sModelKeyframeTranslation
	{
		int		_time;			/// 时刻
		float	_v[3];			/// 平移量
	};

	/// 关键帧旋转参数
	struct sModelKeyframeRotation
	{
		int		_time;			/// 时刻
		float	_q[4];			/// 旋转量
	};

	/// 关键帧缩放参数
	struct sModelKeyframeScale
	{
		int		_time;			/// 时刻
		float	_v[3];			/// 缩放量
	};

	struct _EuclidExport_ sBone
	{
	public:
		sBone()
			:_updatedMtx(Mat4::IDENTITY), _finalMtx(Mat4::IDENTITY)//, _cMtx(Mat4::IDENTITY)
		{
// 			_translation.clear();
// 			_rotation.clear();
// 			_scale.clear();
			_inheritParentTM = true;
			_dynamic = Mat4::IDENTITY;
			memset(_name, 0, 256);
			_translation.setStaticData(Vec3(0.0f, 0.0f, 0.0f));
			_rotation.setStaticData(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
			_scale.setStaticData(Vec3(1.0f, 1.0f, 1.0f));
		}
	public:
		void setName(const std::string& name)
		{
			//_name = name;
			memcpy(_name, name.c_str(), name.size());
		}

	public:
		sKeyFrames<Quaternion>	_rotation;			/// 旋转关键帧
		sKeyFrames<Vec3>		_scale;				/// 缩放关键帧
		sKeyFrames<Vec3>		_translation;			/// 平移关键帧						/// 骨骼ID
// 		bool _notInheritTranslation;			/// 不继承平移
// 		bool _notInheritRotation;				/// 不继承旋转
// 		bool _notInheritScaling;				/// 不继承缩放
// 		bool _hasBillboarded;						/// 有公告板
// 		bool _billboardedLockX;
// 		bool _billboardedLockY;
// 		bool _billboardedLockZ;
// 		bool _cameraAnchored;					/// 是否计算相机锚点
		bool _inheritParentTM;

		Vec3 _pivotPoint;						/// 轴点
		Vec3 _pivotPointTransformed;						/// 轴点

		//骨骼位于初始状态的矩阵，位移部分跟pivotPoint一致，为了方便起见，保留pivotPoint成员
		//此矩阵也就是Maya中的骨骼初始状态下的inclusiveMatrix
		Mat4	_initialMatrix;

		//bool	_pivotRotation;
	public:
		Mat4	_updatedMtx;
		Mat4	_finalMtx;
		//Mat4	_cMtx;
		Mat4	_dynamic;
		//std::string		_name;
		char	_name[256];
		int				_objectId;
		int				_parentId;							/// 父骨骼ID
	public:
		Vec3 getPivot()
		{
			return _pivotPoint;
		}

		Vec3 getPivotTransformed()
		{
			return _pivotPointTransformed;
		}

		Mat4 getInitMatrix()
		{
			return _initialMatrix;
		}
		Mat4 getFinalMatrix()
		{
			return _finalMtx;
		}
	};
//#pragma pack(pop)

	class _EuclidExport_ EntityYZ : public RenderableObject
	{
		friend class SubEntityYZ;
	public:
		typedef u32 Tag;
		typedef u32 Size;
	public:
		EntityYZ(EntityYZPara* p);
		~EntityYZ();
	public:
		virtual bool create();

		virtual void destroy();

		virtual void preRender();
		virtual void postRender();
		//
		virtual void update(u32 current, u32 delta);
		//
		virtual void renderNormal();
		virtual void renderWithEffect();

	public:
		bool setAnimation(const std::string& name);
		void setSpeed(Real speed);
		size_t getBoneNum();
		sBone* getBone(int i);
		Vec3 getBonePivot(int i);
		size_t getSubEntityNum();
		VertexBuffer* cloneVB();
	public:
		bool _decodeFBImpl(const std::string& path);
		bool _decodeFB();
		bool _decodeMZI();
		bool _decodeMZ();
		void calcAABB();
		// should be called after the function calcAABB
		void calcSphere();
		bool _iniIB();
		bool _initVB();
		bool _initSkeleton();
		void _addBoneChild(BoneNode* n, sBone* b);
	private:
		friend class SubEntityYZInstance;
		friend class EntityYZInstance;
		EntityYZPara*		_para;
		
		AnimationMap		_am;
		typedef std::vector<sAnimation> AnimationSet;
		AnimationSet		_animationSet;
		AnimationTime		_clock;
		u32					_version;
		
		SubMeshMap			_sm;

		bool		_hasColor;
		std::vector<sVertexEx*>		_vertices;

		AABB				_aabb;
		Sphere				_sphere;

		std::vector<sFace*>	_faces;

		std::vector<EntityYZMaterial*>	_materials;

		std::vector<sBone*>		_bones;

		std::vector<std::string>	_fbs;

		std::vector<Skeleton*>		_skeletons;

		typedef std::map<std::string, std::string> NamePathVector;
		NamePathVector				_np;
		Real						_speed;
		std::vector<sParticleEmitter*>	_pe;
		std::vector<Mat4>		_matrices;
		std::vector<SubEntityYZ*>	_subEntities;
	};
}

#endif // __EntityYZ_h__
 
