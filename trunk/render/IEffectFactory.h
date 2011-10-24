/********************************************************************
**	file: 		IEffectFactory.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-2-24
**	purpose:	
*********************************************************************/
#ifndef __IEffectFactory_h__
#define __IEffectFactory_h__

#include "Common.h"
#include "IEffect.h"

//
namespace Euclid
{
	class IEffectFactory
	{
	public:
		//
		virtual IEffect*		createEffectFromFile(const std::string& f) = 0;
		//
		virtual IEffect*		createEffectFromMemory(const u8 *pBuffer,u8 len) = 0;
	};
}

#endif // __IEffectFactory_h__ 
