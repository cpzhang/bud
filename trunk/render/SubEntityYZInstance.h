/********************************************************************
**	file: 		SubEntityYZInstance.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-6
**	purpose:	
*********************************************************************/
#ifndef __SubEntityYZInstance_h__
#define __SubEntityYZInstance_h__

#include "Common.h"
#include "RenderableObject.h"

namespace Euclid
{
//#	pragma pack( push, packing )
//#	pragma pack( 1 )

	class _EuclidExport_ SubEntityYZInstancePara : public RenderablePara
	{
	public:
		SubEntityYZInstancePara()
			:	_name(""), _entityYZInstance(NULL), _subEntityName("")
		{

		}

		SubEntityYZInstancePara(SubEntityYZInstancePara* p)
		{
			if (p != NULL)
			{
				_name = p->_name;
				_entityYZInstance = p->_entityYZInstance;
				_subEntityName = p->_subEntityName;
			}
		}

	public:
		std::string		_name;
		EntityYZInstance*		_entityYZInstance;
		std::string		_subEntityName;
	};
//# pragma pop
	class _EuclidExport_ SubEntityYZInstance : public RenderableObject
	{
	public:
	public:
		SubEntityYZInstance(SubEntityYZInstancePara* p);
		~SubEntityYZInstance();
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
		
	private:
		SubEntityYZInstancePara*	_para;
	};
}

#endif // __SubEntityYZInstance_h__
