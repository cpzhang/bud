#pragma  once
#include "IPrimitive.h"
#include "Event.h"
class EventArgsCTreeDlgChangeFile: public EventArgs
{
public:
	IPrimitive*		   mPrimitive;
	eVisibleObjectType mType;
};