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

namespace Zen
{
	class _ZenExport_ BufferManager : public Zen::SingletonEx<BufferManager>
	{
	public:
		BufferManager();
		~BufferManager();

		//
	public:
		void onInvalidateDevice();
		void onRestoreDevice();
	public:
		virtual IBuffer*	createVertexBuffer(unsigned int Length,	eUsage Usage, ePool Pool);
		virtual IBuffer*	createIndexBuffer(unsigned int Length,	eUsage Usage, eFormat Format,	ePool Pool);
	private:
		std::vector<IBuffer*>	_buffers;
	};
}

#endif // __BufferManager_h__