#pragma once
#include "base/Buddha.h"
#include "math/Euler.h"
#include "render/Euclid.h"
#include <string>
enum eVisibleObjectType
{
	eVisibleObjectType_SubEntity,
	eVisibleObjectType_Entity,
	eVisibleObjectType_MzModel,
	eVisibleObjectType_Group,
};
//
struct sAnimation
{
	std::string name;
	int			duration;
	std::string skinFile;
};
typedef std::map<std::string, sAnimation> AniMap;
typedef	ConstBaseIterator<AniMap>	AniMapIterator;

class IPrimitive
{
#define IPrimitiveInterface(terminal)\
public:\
	virtual void decode(const std::string& fileName) ##terminal\
	virtual void render() ##terminal\
	virtual void destroy() ##terminal\
	virtual void update(u32 delta) ##terminal\
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix ) ##terminal\

#define IPrimitive_Base IPrimitiveInterface(=0;)
#define IPrimitive_Derived IPrimitiveInterface(;)
	IPrimitive_Base
public:
	virtual ~IPrimitive(){};

};