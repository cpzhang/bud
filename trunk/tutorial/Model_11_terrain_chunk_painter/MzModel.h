#pragma once
#include "IPrimitive.h"
class Entity;
class MzModel: public IPrimitive
{
public:
	MzModel();
	~MzModel();
	IPrimitive_Derived
private:
	void _clear();
	void saveFile(const std::string& fileName);
private:
	Entity* mEntity;
};