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
namespace Euclid
{
	class EffectManager : public Buddha::SingletonEx<EffectManager>
	{
	public:
		//
		virtual Effect*		createEffectFromFile(const std::string& f);
		//
		virtual Effect*		createEffectFromMemory(const u8 *pBuffer,u8 len);
	};
}

#endif // __EffectManager_h__