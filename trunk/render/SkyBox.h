//==========================================================================
/**
* @file	  : SkyBox.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-23   14:41
* purpose : SkyBox
*/
//==========================================================================

#ifndef __SkyBox_h__
#define __SkyBox_h__

#include "Common.h"
#include "RenderableObject.h"
#include "VertexDeclaration.h"
//
namespace Euclid
{
	class SkyBox : public RenderableObject
	{
	public:
		SkyBox();
		~SkyBox();

	public:
		enum eSkyBoxPlane
		{
			eSkyBoxPlane_X_Positive = 0,
			eSkyBoxPlane_X_Negative,

			eSkyBoxPlane_Y_Positive,
			eSkyBoxPlane_Y_Negative,

			eSkyBoxPlane_Z_Positive,
			eSkyBoxPlane_Z_Negative,

			eSkyBoxPlane_Max,
		};
		//
	public:
		//
		virtual bool create();
		//
		virtual void destroy();

		//
		virtual void render();

		//
		virtual void preRender();

		//
		virtual void postRender();

		//
	private:
		bool _initGeometry();

		//
	private:
		static const POSITION_TEXTURE	cGeometry[eSkyBoxPlane_Max][4];
		static const char				cTextureFileName[eSkyBoxPlane_Max][10];
		ITexture*						_tex[eSkyBoxPlane_Max];
	};
}

#endif // __SkyBox_h__
 
