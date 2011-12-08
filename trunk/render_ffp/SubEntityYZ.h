/********************************************************************
**	file: 		SubEntityYZ.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-3-22
**	purpose:	
*********************************************************************/
#ifndef __SubEntityYZ_h__
#define __SubEntityYZ_h__

#include "Common.h"
#include "RenderableObject.h"

namespace Euclid
{
	class SubEntityYZ  : public RenderableObject
	{
	public:
		SubEntityYZ(EntityYZ* entity);
		~SubEntityYZ();
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

	private:
		EntityYZ*	_entity;
	};
}
#endif // __SubEntityYZ_h__
