/********************************************************************
**	file: 		BufferManager.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-12-9
**	purpose:	
*********************************************************************/
#ifndef __BufferManager_h__
#define __BufferManager_h__

#include "Common.h"
#include "RenderEngineCreationParameters.h"

namespace Euclid
{
	class BufferManager : public Buddha::SingletonEx<BufferManager>
	{
	public:
		BufferManager();
		~BufferManager();

		//
	public:
	public:
		virtual IBuffer*	createVertexBuffer(unsigned int Length,	unsigned long Usage, ePool Pool);
		virtual IBuffer*	createIndexBuffer(unsigned int Length,	unsigned long Usage, D3DFORMAT Format,	ePool Pool);
	};
}

#endif // __BufferManager_h__