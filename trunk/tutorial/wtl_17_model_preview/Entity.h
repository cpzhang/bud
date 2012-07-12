#pragma once
#include "IPrimitive.h"
class Geometry;
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
	Geometry* mGeometry;
	AniMap mAnimations;
};