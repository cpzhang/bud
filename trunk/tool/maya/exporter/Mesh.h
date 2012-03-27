#pragma once
#include "submesh.h"
#include "skeleton.h"

class Mesh
{
public:
	Mesh(void);
	virtual ~Mesh(void);

	void clear();
	MStatus load(MDagPath& meshDag,MDagPath *pDagPath);
public:
	std::vector<uvset> m_uvsets;
	std::vector<SubMesh*> m_submeshes;
	MFnSkinCluster* m_pSkinCluster;
	skeleton* m_pSkeleton;
};
