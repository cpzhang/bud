/********************************************************************
**	file: 		EffectManager.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __EffectManager_h__
#define __EffectManager_h__
#include "Common.h"
//
namespace Zen
{
	class _ZenExport_ EffectManager : public Zen::SingletonEx<EffectManager>
	{
	public:
		//
		virtual Effect*		createEffectFromFile(const std::string& f);
		virtual Effect*		getEffectByFile(const std::string& f);
		//
		virtual Effect*		createEffectFromMemory(const u8 *pBuffer,u8 len);
		//
		void onInvalidateDevice();
		void onRestoreDevice();
		void OnResetDevice();
		void onDestroyEffect(Effect* e);
	private:
		typedef std::map<std::string, Effect*> NameEffectMap;
		NameEffectMap _effects;
	};
}

#endif // __EffectManager_h__