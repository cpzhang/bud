#pragma once
#include "IPrimitive.h"
class Geometry;
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
private:
	void _clear();
private:
	typedef std::vector<Geometry*> GeoVec;
	GeoVec mGeometries;
};
