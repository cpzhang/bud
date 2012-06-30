/********************************************************************
**	file: 		IMaterial.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __IMaterial_h__
#define __IMaterial_h__
#include "Common.h"
#include "RenderEngineCreationParameters.h"
#include "Color.h"
#include "math/Quaternion.h"
#include "KeyFrames.h"
namespace Euclid
{
	enum eMaterialType
	{
		eMaterialType_Vertex,
		eMaterialType_VertexTexture,
		eMaterialType_VertexSkin,

		eMaterialType_Size,
	};

	class _EuclidExport_ IMaterial
	{
	public:
		IMaterial()
		{
			_clear();
		}
		~IMaterial()
		{

		}
		//
		void setVertexDeclaration(eVertexDeclarationType e);
		//
		virtual u32 getStride() = 0;
		//
		virtual void apply();
		//
		virtual void destroy();
		//
		virtual void setEffect(const tstring& name);
		virtual Effect* getEffect(){return _effect;};
		virtual bool setTexture(const tstring& name, const tstring& fileName){return true;};
		void loadAnimation(const tstring& name);
		void update(u32 delta);
	private:
		void _clear()
		{
			_effect = NULL;
			mZCmpFunc = eCmpFunc_Less;
			mDiffuse = Color::White;
			mUseVertexColor = false;
			mUVMatrix = Mat4::IDENTITY;
			_zEnable = eZBufferType_True;
			_zWriteEnable = true;
			_alphaTestEnable = false;
			_srcBlend = eBlend_One;
			_destBlend = eBlend_Zero;
			mAlphaCmpFunc = eCmpFunc_Always;
			mAlphaRef = 0;
			mZCmpFunc = eCmpFunc_LessEqual;
			mAlphaBlendEnable = false;
			mFillMode = eFillMode_Solid;
			mCullMode = eCullMode_None;
		}
	public:
		char mVersion;
		eVertexDeclarationType	_vertexDeclaration;
		Effect*					_effect;
		eZBufferType			_zEnable;
		bool					_zWriteEnable;
		bool					_alphaTestEnable;
		eBlend					_srcBlend;
		eBlend					_destBlend;
		eCmpFunc				mAlphaCmpFunc;
		u32						mAlphaRef;
		eCmpFunc				mZCmpFunc;
		bool					mAlphaBlendEnable;
		Color					mDiffuse;
		sKeyFrames<Quaternion>	mRotationKFs;			/// 旋转关键帧
		sKeyFrames<Vec3>		mScaleKFs;				/// 缩放关键帧
		sKeyFrames<Vec3>		mTranslationKFs;		/// 平移关键帧
		sKeyFrames<Color3>		mColorKFs;		/// 颜色关键帧
		sKeyFrames<float>		mAlphaKFs;		/// 透明度关键帧
		AnimationTime			mAlphaAnimationTime;
		bool					mUseVertexColor;
		Mat4					mUVMatrix;
		eFillMode				mFillMode;
		eCullMode				mCullMode;
	};

	class IMaterialManager
	{
	public:
		virtual IMaterial* createMaterial(eMaterialType e) = 0;
	};
}


#endif // __IMaterial_h__