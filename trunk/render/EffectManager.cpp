#include "EffectManager.h"
#include "Effect.h"

//
namespace Euclid
{
	Effect* Euclid::EffectManager::createEffectFromFile( const tstring& f )
	{
		NameEffectMap::iterator it = _effects.find(f);
		if (it != _effects.end())
		{
			it->second->addReference();
			return it->second;
		}
		Effect *pEffect = new Effect;
		if(!pEffect->loadFromFile(f))
		{
			delete pEffect;
			return NULL;
		}
		_effects[f] = pEffect;
		pEffect->addReference();
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

	void EffectManager::onDestroyEffect( Effect* e )
	{
		NameEffectMap::iterator it = _effects.begin();
		for(; it != _effects.end(); ++it)
		{
			if (e == it->second)
			{
				delete e;
				_effects.erase(it);
				return;
			}
		}
	}

}