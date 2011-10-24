/********************************************************************
**	file: 		EntityYZInstance.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-3
**	purpose:	
*********************************************************************/
#ifndef __EntityYZInstance_h__
#define __EntityYZInstance_h__

#include "Common.h"
#include "RenderableObject.h"
#include "KeyFrames.h"

namespace Euclid
{
//#	pragma pack( push, packing )
//#	pragma pack( 1 )

	class _EuclidExport_ EntityYZInstancePara : public RenderablePara
	{
	public:
		EntityYZInstancePara()
			:	_name(""), _entityYZ(NULL)
		{

		}

		EntityYZInstancePara(EntityYZInstancePara* p)
		{
			if (p != NULL)
			{
				_name = p->_name;
				_entityYZ = p->_entityYZ;
			}
		}

	public:
		std::string		_name;
		EntityYZ*		_entityYZ;
	};
//# pragma pop
	class _EuclidExport_ EntityYZInstance : public RenderableObject
	{
	public:
		EntityYZInstance(EntityYZInstancePara* p);
		~EntityYZInstance();
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
		void setRecycle(bool b);
		size_t getSubEntityYZInstanceNum();
		SubEntityYZInstance* getSubEntityInstance(size_t i);
		bool isRenderingBone();
		void setRenderingBone(bool b);
		void clear();
	private:
		void updateClock(u32 delta);
	private:
		friend class SubEntityYZInstance;
		EntityYZInstancePara*	_para;
		typedef std::vector<SubEntityYZInstance*> SubEntityYZInstanceSet;
		SubEntityYZInstanceSet	_subEntityInstances;
		AnimationTime				_clock;
		Real						_speed;
		bool						_isRecycle;
		bool						_isRenderingBone;
		ManualObject*				_bonesObject;
	};
}

#endif // __EntityYZInstance_h__
