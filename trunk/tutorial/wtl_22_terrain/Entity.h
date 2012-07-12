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
	virtual AniMapIterator getAniMapIterator(void) const;
	virtual void setAnimationName(const std::string& fileName);
	virtual void stopAnimation();
	virtual void playAnimation();
	virtual void setSpeed(float speed);
private:
	void _clear();
private:
	std::string mName;
	typedef std::vector<SubEntity*> SubEntityVec;
	SubEntityVec mSubEntities;
};