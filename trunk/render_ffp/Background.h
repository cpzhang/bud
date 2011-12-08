//==========================================================================
/**
* @file	  : Background.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-13   18:32
* purpose : 
*/
//==========================================================================

#ifndef __Background_h__
#define __Background_h__

#include "Common.h"
#include "RenderableObject.h"
#include "VertexDeclaration.h"

namespace Euclid
{
	class  BackgroundPara: public RenderablePara
	{
	public:
		std::string		_texName;
	};

	class Background : public RenderableObject
	{
	public:
		//
		virtual bool create();
		//
		virtual void destroy();

		//
		virtual void update(u32 current, u32 delta);

		//
		virtual void render();

		//
		virtual void preRender();

		//
		virtual void postRender();

	public:
		Background(const BackgroundPara& p);
		~Background();

		//
	private:
		BackgroundPara					_para;
		static TPOSITION_TEXTURE		_vertex[4];
		ITexture*						_tex;
	};
}


#endif // __Background_h__
 
