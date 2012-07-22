#pragma once
#include "base/Buddha.h"
#include "math/Euler.h"
#include "render/Euclid.h"
class Chunks;
bool isInsideCircle(Real x, Real y, Real ox, Real oy, Real radius);
class Sculptor
{
public:
	Sculptor();
	~Sculptor();
	void create();
	bool isInside(const Vec3& p);
	void move(const Vec3& p, Chunks* cks);
	void destroy();
	void render();
	void generateMesh(Real radius, Chunks* cks);
private:
	void _clear();
public:
	Real mRadius;
	Vec3 mOrigion;
	Real mParaA;
	Zen::IMaterial* mMaterial;
	std::vector<Vec3> mVertices;
};