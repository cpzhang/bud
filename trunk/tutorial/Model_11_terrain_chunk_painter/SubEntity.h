#pragma once
#include "IPrimitive.h"
class Geometry;
class SubEntity : public IPrimitive
{
public:
	SubEntity();
	~SubEntity();

	IPrimitive_Derived
private:
	void _clear();
public:
	std::string mName;
	Geometry* mGeometry;
	AniMap mAnimations;
	eVisibleObjectType mType;
};