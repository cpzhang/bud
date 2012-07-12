#pragma once
#include "IPrimitive.h"
class SubEntity;
class Entity : public IPrimitive
{
public:
	Entity();
	~Entity();
	virtual void decode(const std::string& fileName);
	virtual void render();
	virtual void update(u32 delta);
	virtual void destroy();
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix );
private:
	void _clear();
private:
	std::string mName;
	typedef std::vector<SubEntity*> SubEntityVec;
	SubEntityVec mSubEntities;
};