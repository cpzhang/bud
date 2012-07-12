#pragma once
#include <string>
#include <map>
#include "math/Euler.h"
#include "base/ConstBaseIterator.h"
#include "AABBEntity.h"
struct sAnimation
{
	std::string name;
	int duration;
	std::string skinFile;
};
typedef std::map<std::string, sAnimation> AniMap;
typedef	ConstBaseIterator<AniMap>	AniMapIterator;
class IPrimitive
{
public:
	IPrimitive()
	{
		mAABBRendable = false;
	}
	virtual ~IPrimitive(){};
	//È«Â·¾¶
	virtual void decode(const std::string& fileName) = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix ) = 0;
	virtual void saveFile(const std::string& fileName){};
	virtual void renderSkeleton(){};
	virtual void update(u32 delta){};
	virtual AniMapIterator getAniMapIterator(void) const = 0;
	virtual void setAnimationName(const std::string& fileName) = 0;
	virtual void stopAnimation(){};
	virtual void playAnimation(){};
	virtual void setSpeed(float speed){};
	virtual bool intersect(const Ray& r, Real& p)
	{
		return false;
	}
	void destroyAABB()
	{
		mAABB.destroyMaterial();
	}
	
protected:
	void renderAABB()
	{
		if (mAABBRendable)
		{
			mAABB.render();
		}
	};
	void setAABBMatrix( const std::string& name, const Mat4& pMatrix )
	{
		mAABB.setMatrix(name, pMatrix);
	}
protected:
	AABBEntity mAABB;
	bool mAABBRendable;
};
