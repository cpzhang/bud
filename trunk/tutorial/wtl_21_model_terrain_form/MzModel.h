#pragma once
#include "IPrimitive.h"
class Entity;
class MzModel: public IPrimitive
{
public:
	MzModel();
	~MzModel();
	virtual void saveFile(const std::string& fileName);
	virtual void decode(const std::string& fileName);
	virtual void render();
	virtual void destroy();
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix );
	virtual void update(u32 delta);
	virtual AniMapIterator getAniMapIterator(void) const;
	virtual void setAnimationName(const std::string& fileName);
private:
	void _clear();
private:
	Entity* mEntity;
};
