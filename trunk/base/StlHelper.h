//==========================================================================
/**
* @file	  : StlHelper.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-24   10:31
* purpose : 
*/
//==========================================================================

#ifndef __StlHelper_h__
#define __StlHelper_h__

#include "Common.h"

#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}

template<class T>void STLDeleteAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}

template<class T>void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE(*i);
	}
	t.clear();
}

template<class T>void STLReleaseAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_RELEASE((*i).second);
	}
	t.clear();
}
template<class T>void STLDestroySequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDestroy(*i);
	}
	t.clear();
}

template<class T>void STLDestroyAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDestroy((*i).second);
	}
	t.clear();
}

template<class T>void STLReleaseSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		(*i)->release();
	}
	t.clear();
}

template<class T,class TS>void STLDeleteAssociateSequence(T& t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		TS *pTS = (*i).second;
		if(pTS)
		{
			STLDeleteSequence(*pTS);
		}
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}

#endif // __StlHelper_h__