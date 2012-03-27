#include "EffectManager.h"
#include "Effect.h"

//
namespace Euclid
{
	Effect* Euclid::EffectManager::createEffectFromFile( const std::string& f )
	{
		NameEffectMap::iterator it = _effects.find(f);
		if (it != _effects.end())
		{
			return it->second;
		}
		Effect *pEffect = new Effect;
		if(!pEffect->loadFromFile(f))
		{
			delete pEffect;
			return NULL;
		}
		_effects[f] = pEffect;
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

	void EffectManager::onInvalidateDevice()
	{
		for (NameEffectMap::iterator i = _effects.begin(); i != _effects.end(); ++i)
		{
			Effect* e = i->second;
			if (e)
			{
				e->onInvalidateDevice();
			}
		}
	}

	void EffectManager::onRestoreDevice()
	{
		for (NameEffectMap::iterator i = _effects.begin(); i != _effects.end(); ++i)
		{
			Effect* e = i->second;
			if (e)
			{
				e->onRestoreDevice();
			}
		}
	}

	void EffectManager::OnResetDevice()
	{
		for (NameEffectMap::iterator i = _effects.begin(); i != _effects.end(); ++i)
		{
			Effect* e = i->second;
			if (e)
			{
				e->onResetDevice();
			}
		}
	}
}