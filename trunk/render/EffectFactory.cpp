#include "stdafx.h" 
#include "EffectFactory.h"
#include "Effect.h"

//
namespace Euclid
{
	IEffect* Euclid::EffectFactory::createEffectFromFile( const std::string& f )
	{
		Effect *pEffect = new Effect;
		if(!pEffect->loadFromFile(f))
		{
			delete pEffect;
			return NULL;
		}
		return pEffect;
	}

	IEffect* Euclid::EffectFactory::createEffectFromMemory( const u8 *pBuffer,u8 len )
	{
		Effect *pEffect = new Effect;
		if(!pEffect->loadFromMemory(pBuffer, len))
		{
			delete pEffect;
			return NULL;
		}
		return pEffect;
	}
}