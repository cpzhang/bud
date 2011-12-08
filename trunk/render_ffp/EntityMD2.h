//==========================================================================
/**
* @file	  : EntityMD2.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-17   14:22
* purpose : 
*/
//==========================================================================

#ifndef __EntityMD2_h__
#define __EntityMD2_h__

#include "Common.h"
#include "RenderableObject.h"

namespace Euclid
{
	class EntityMD2 : public RenderableObject
	{
	public:
		//
		virtual void update();
		virtual bool create();
		virtual void destroy();
		virtual void preRender();
		virtual void postRender();
		//
		virtual void render();

		//
	public:
		EntityMD2();
		~EntityMD2();

		//
	public:
		bool load(const std::string& fileName);
		void setFrameIndex(int index);
		//
	private:
		MD2Loader*		_md2;
		MD3Loader*		_md3;
		int				_currentIndex;
	};
} // Euclid

#endif // __EntityMD2_h__
 
