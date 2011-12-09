#include "EffectManager.h"
#include "Effect.h"

//
namespace Euclid
{
	Effect* Euclid::EffectManager::createEffectFromFile( const std::string& f )
	{
		Effect *pEffect = new Effect;
		if(!pEffect->loadFromFile(f))
		{
			delete pEffect;
			return NULL;
		}
		return pEffect;
	}

	Effect* Euclid::EffectManager::createEffectFromMemory( const u8 *pBuffer,u8 len )
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