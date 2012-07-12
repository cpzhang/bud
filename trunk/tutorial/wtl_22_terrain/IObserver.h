#pragma once
class IPrimitive;

class IObserver
{
public:
	virtual void update(IPrimitive* p) = 0;
};
