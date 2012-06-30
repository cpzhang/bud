#include "IMaterial.h"
#include "RenderSystem.h"
#include "Effect.h"
#include "EffectManager.h"
namespace Euclid
{
	//
	void IMaterial::setVertexDeclaration(eVertexDeclarationType e)
	{
		_vertexDeclaration = e;
	}
	//
	void IMaterial::apply()
	{
		RenderSystem::getInstancePtr()->setVertexDeclaration(_vertexDeclaration);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_ZEnable, _zEnable);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_ZWriteEnable, _zWriteEnable);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_ZFunc, mZCmpFunc);

		RenderSystem::getInstancePtr()->setRenderState(eRenderState_AlphaTestEnable, _alphaTestEnable);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_SrcBlend, _srcBlend);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_DestBlend, _destBlend);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_AlphaFunc, mAlphaCmpFunc);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_AlphaRef, mAlphaRef);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_FillMode, mFillMode);
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_CullMode, mCullMode);
		if (_srcBlend == eBlend_One && _destBlend == eBlend_Zero)
		{
			mAlphaBlendEnable = false;
		}
		else
		{
			mAlphaBlendEnable = true;
		}
		RenderSystem::getInstancePtr()->setRenderState(eRenderState_AlphaBlendEnable, mAlphaBlendEnable);
	}

	void IMaterial::destroy()
	{
		if (_effect)
		{
			// 
			_effect->destroy();
		}
		_clear();
	}

	void IMaterial::setEffect( const tstring& name )
	{
		_effect = EffectManager::getInstancePtr()->createEffectFromFile(name);
	}

	void IMaterial::loadAnimation( const tstring& name )
	{
		std::ifstream f(name.c_str(), std::ios::binary);
		if (!f.good())
		{
			return;
		}

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
					f.read(&mVersion, s);
				}
				break;

			case 'MALP':
				{
					u32 nKeyframes;
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					sKeyFrame<float> kf;
					for (size_t i = 0; i != nKeyframes; ++i)
					{
						f.read((char*)&kf, sizeof(kf));
						mAlphaKFs.addKeyFrame(kf);
					}
					mAlphaAnimationTime.end = kf.time;
				}
				break;
			case 'MCOL':
				{
					u32 nKeyframes;
					f.read((char*)&nKeyframes,sizeof(nKeyframes));
					sKeyFrame<Color3> kf;
					for (size_t i = 0; i != nKeyframes; ++i)
					{
						f.read((char*)&kf, sizeof(kf));
						mColorKFs.addKeyFrame(kf);
					}
					//mAlphaAnimationTime.end = kf.time;
				}
				break;
			default:
				{
					f.ignore(s);
					Error("遇到无法识别的数据块");
				}
				break;
			}
		}
		f.close();
	}

	void IMaterial::update( u32 delta )
	{
		if (mAlphaAnimationTime.end > mAlphaAnimationTime.start)
		{
			mAlphaAnimationTime.current += delta;
			while(mAlphaAnimationTime.current > mAlphaAnimationTime.end)
			{
				mAlphaAnimationTime.current -= mAlphaAnimationTime.end;
			}
		}
		mUVMatrix = Mat4::IDENTITY;
		if (mRotationKFs.numKeyFrames())
		{
			//
			Quaternion q = mRotationKFs.getFrame(0);
			Mat4 tQ(q);
			Mat4 tT = Mat4::IDENTITY;
			Vec3 offsetMatrix(-0.5, -0.5, 0.0);
			tT.setTrans(offsetMatrix);
			mUVMatrix = tT.inverse() * tQ * tT;
		}
		if (mScaleKFs.numKeyFrames())
		{
			//
			Vec3 speed(0.000, 0.000, 0.0);
			Quaternion q(0, 0, 0, 0.00);
			Mat4 tQ(q);
			Mat4 tT = Mat4::IDENTITY;
			Vec3 offsetMatrix(-0.5, -0.5, 0.0);
			tT.setTrans(offsetMatrix);
			mUVMatrix = tT.inverse() * tQ * tT;
		}
		if (mTranslationKFs.numKeyFrames())
		{
			//
			Vec3 speed(0.000, 0.000, 0.0);
			Quaternion q(0, 0, 0, 0.00);
			Mat4 tQ(q);
			Mat4 tT = Mat4::IDENTITY;
			Vec3 offsetMatrix(-0.5, -0.5, 0.0);
			tT.setTrans(offsetMatrix);
			mUVMatrix = tT.inverse() * tQ * tT;
		}
	}
}