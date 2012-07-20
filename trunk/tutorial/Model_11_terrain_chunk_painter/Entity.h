#pragma once
#include "IPrimitive.h"
class SubEntity;
class Entity : public IPrimitive
{
public:
	Entity();
	~Entity();
	IPrimitive_Derived

private:
	void _clear();
public:
	std::string mName;
	typedef std::vector<SubEntity*> SubEntityVec;
	SubEntityVec mSubEntities;
	eVisibleObjectType mType;
};