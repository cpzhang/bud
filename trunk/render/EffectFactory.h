/********************************************************************
**	file: 		EffectFactory.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-2-24
**	purpose:	
*********************************************************************/
#ifndef __EffectFactory_h__
#define __EffectFactory_h__

#include "IEffectFactory.h"
class IEffect;
//
namespace Euclid
{
	class EffectFactory : public IEffectFactory, public Buddha::SingletonEx<EffectFactory>
	{
		//
		virtual IEffect*		createEffectFromFile(const std::string& f);
		//
		virtual IEffect*		createEffectFromMemory(const u8 *pBuffer,u8 len);
	};
}

#endif // __EffectFactory_h__ 
