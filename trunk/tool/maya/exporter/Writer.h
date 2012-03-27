#pragma once
#include "mesh.h"
#include "materialset.h"
#include "MHeader.h"
#include "ExportOptions.h"

class MWriter
{
public:
	MWriter(void);
	virtual ~MWriter(void);

public:
	MStatus read();
	MStatus write();
	void clear();
private:
	MStatus translateNode(MDagPath& dagPath,MDagPath *pDagPath);
	void loadMtlKeyframes();

private:
	Mesh* m_pMesh;
	skeleton* m_pSkeleton;
	MaterialSet* m_pMaterialSet;
};