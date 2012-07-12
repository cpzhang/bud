#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
//
// Windows 头文件:
#include <windows.h>
#include "windowsx.h"
#include "Commdlg.h"
#include "process.h"
#include "shlwapi.h"
#include "comdef.h"

#include "atlstr.h"
#include <atlcoll.h>
#include <bitset>

//
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <algorithm>

//
#include "plugapi.h"
//#include "CoreFunctions.h"
#include "coreexp.h"
#include "iparamb2.h"
#include "inode.h"
#include "impexp.h"
#include "impapi.h"
#include "triobj.h"
#include "stdmat.h"
#include "iskin.h"
#include "modstack.h"
#include "istdplug.h"
#include "CS/Phyexp.h"
#include "CS/bipedapi.h"
#include "decomp.h"
#include "contextids.h"
#include "helpsys.h"
//
#include "IGame/IGame.h"
#include "IGame/IConversionManager.h"
#include "XRef/iXrefObj.h"
#include "XRef/iXrefItem.h"
#include "XRef/iXrefMaterial.h"

#include "resource.h"
//#include "buddha.h"

//
//#include "MHeader.h"


#pragma   pack(1)
template<class T> struct keyframeT
{
	int time;
	T data;
	keyframeT(const keyframeT& rk)
	{
		time = rk.time;
		data = rk.data;
	}
	keyframeT()
	{
	}
	inline bool operator == (keyframeT& k)
	{
		return data == k.data;
	}

	inline bool operator != (keyframeT& k)
	{
		return data != k.data;
	}
	inline keyframeT linear(float r, const keyframeT& rk) const
	{
		keyframeT s;
		s.data = r * data + (1 - r) * rk.data;
		
		return s;
	}
};
#pragma   pack()
class MemoryFileWriter;
template<class T>void writeSequence(MemoryFileWriter* pDataStream,T &t)
{
	T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		pDataStream->write(&(*i),sizeof(*i));
	}
}

template<class T> void printSet(std::ostream& out, T& data)
{
	T::iterator i = data.begin();
	for (; i != data.end(); ++i)
	{
		out<<*i;
	}
}

template<class T> bool CheckAllKeyFramesTheSame(T& v)
{
	if (v.empty())
	{
		return false;
	}

	T::iterator it = v.begin();
	++it;
	T::iterator itBegin = v.begin();
	for (; it != v.end(); ++it)
	{
		if (*it != *itBegin)
		{
			return false;
		}
	}

	return true;
}

template<class T> void SampleLinear(T& v, size_t distance)
{
	if (distance >= v.size() - 1)
	{
		return;
	}
	T::iterator currentIt = v.begin();
	std::advance(currentIt, distance);

	T::iterator itBegin = currentIt;
	std::advance(itBegin, -1);
	T::iterator itEnd = currentIt;
	std::advance(itEnd, 1);

	float totalTime = itEnd->time - itBegin->time;
	float currentTime = currentIt->time - itBegin->time;
	if (itBegin->linear(currentTime / totalTime, *itEnd) == *currentIt)
	{
		v.erase(currentIt);
	}
	else
	{
		++distance;
	}

	SampleLinear(v, distance);
}

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef float        float3[3];
typedef signed short short3[3];

#define safeDestroy(x) if((x)!=NULL){ (x)->destroy(); delete (x); (x) = NULL;}
#define safeDelete(x) if((x)!=NULL){delete (x); (x) = NULL;}

#define safeDeleteArray(x) if((x)!=NULL){delete[] (x); (x) = NULL;}
#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}

template<class T>void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE(*i);
	}
	t.clear();
}

extern HINSTANCE gInstance; 

#define DIY_MATERIAL

#define SwitchYZ 1

#define breakable if (false){}

//#define Check_Biped_Bone
