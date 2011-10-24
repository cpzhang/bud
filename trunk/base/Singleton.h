//==========================================================================
/**
* @file	  : SingletonEx.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-3-25   12:11
* purpose : Singleton Template
*/
//==========================================================================

#ifndef __SingletonEx_h__
#define __SingletonEx_h__
//
#include "Common.h"

namespace Buddha
{
	/// 扩展的单实体模板，不关心对象的创建和销毁
	/// 采用外部new和delete，这种单实体的好处是外部能控制构造和析构的顺序
	template <typename T>
	class SingletonEx
	{
	protected:
		static T*	_instance;

	public:
		SingletonEx()
		{
#	if _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(SingletonEx<T>*)(T*)1;
			_instance = (T*)((int)this + offset);
#	else
			_instance = static_cast<T*>(this);
#	endif
		}

		~SingletonEx()
		{
			_instance = 0;
		}

		static T& getInstance()		{ return (*_instance); }
		static T* getInstancePtr()	{ return _instance; }
	};

	template <typename T> T* SingletonEx<T>::_instance = 0;
}

#endif // __SingletonEx_h__