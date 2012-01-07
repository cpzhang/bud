#include "RainParticleDraw.h"
#include "RainParticle.h"


BOOL RainParticleDraw::DrawParticle( GraphicsWindow *gw,ParticleSys &parts,int i )
{
	/*
		The number of points in the polyline. The maximum number of points that may be used in drawing a polyline is 32.
	*/
	int ct = 2;

	/*
		Array of points. This array must be at least one element larger than the ct parameter that is passed in.
		The 3D clipper will use the "extra" space to clip as efficiently as possible. If room for the extra element is not provided, 3ds Max will crash.
	*/
	Point3 pt[3];
	float sz = 0.0f;//CompParticleSize(parts.ages[i], life, parts.size);
	pt[0] = parts[i];
	pt[1] = parts[i] - parts.vels[i] * RAINSIZEFACTOR * sz;

	/*
		If nonzero the first point is connected to the last point, i.e. the polyline is closed.
	*/
	int closed = FALSE;
	gw->polyline(ct, pt, NULL, NULL, closed, NULL);

	if (GetAsyncKeyState (VK_ESCAPE)) return TRUE;
	return 0;
}

BOOL MeshParticleDraw::DrawParticle( GraphicsWindow *gw,ParticleSys &parts,int i )
{
	/*
		Array of points. This array must be at least one element larger than the ct parameter that is passed in.
		The 3D clipper will use the "extra" space to clip as efficiently as possible. If room for the extra element is not provided, 3ds Max will crash.
	*/
	Point3 pt[5];
	float half_size = 20.0f;
	pt[0] = Point3(parts[i].x - half_size, parts[i].y, parts[i].z - half_size);
	pt[1] = Point3(parts[i].x + half_size, parts[i].y, parts[i].z - half_size);
	pt[2] = Point3(parts[i].x - half_size, parts[i].y, parts[i].z + half_size);
	pt[3] = Point3(parts[i].x + half_size, parts[i].y, parts[i].z + half_size);

	pt[4] = parts[i];

	Point3 uvw[7];
	uvw[0] = Point3(0.0f, 0.0f, 0.0f);
	uvw[1] = Point3(1.0f, 0.0f, 0.0f);
	uvw[2] = Point3(0.0f, 1.0f, 0.0f);

	uvw[3] = Point3(1.0f, 1.0f, 0.0f);
	uvw[4] = Point3(0.0f, 0.0f, 0.0f);
	uvw[5] = Point3(0.0f, 0.0f, 0.0f);
	
	uvw[6] = Point3(0.0f, 0.0f, 0.0f);

	gw->triStrip(4, pt, NULL, uvw);

	if (sp)
	{
		Mtl* m = sp->_material;
		if (m)
		{
			//gw->setMaterial(*m, 0);
		}
	}
	if (GetAsyncKeyState (VK_ESCAPE))
	{
		return TRUE;
	}

	return 0;
}

