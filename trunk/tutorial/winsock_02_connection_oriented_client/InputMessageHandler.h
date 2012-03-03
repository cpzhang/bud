/********************************************************************
**	file: 		InputMessageHandler.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011Äê1ÔÂ14ÈÕ
**	purpose:	
*********************************************************************/
#ifndef __InputMessageHandler_h__
#define __InputMessageHandler_h__

#include "base/Buddha.h"

class InputMessageHandler : public Buddha::IInputMessageHandler
{
public:
	InputMessageHandler();
	~InputMessageHandler();

public:
	virtual bool onInputMessage(void* hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
};


#endif // __InputMessageHandler_h__
