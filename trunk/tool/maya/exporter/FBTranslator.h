#pragma once
#include "common.h"
#include "FeiBian.h"
#include "Mesh.h"

class FBTranslator : public MPxFileTranslator
{
public:
	FBTranslator()
	{
		m_pMesh = new Mesh;
		m_pMaterialSet = new MaterialSet;
	}
	virtual ~FBTranslator(void);
	static void* creator();

public:
	MStatus		reader(const MFileObject& file, const MString& ExportOptionsString, MPxFileTranslator::FileAccessMode mode);
	MStatus		writer(const MFileObject& file, const MString& ExportOptionsString, MPxFileTranslator::FileAccessMode mode);
	bool		haveReadMethod() const;
	bool		haveWriteMethod() const;
	bool		canBeOpened() const; 
	MString 	defaultExtension() const;
	MString 	filter() const;
	MFileKind	identifyFile( const MFileObject& fileName, const char* buffer, short size) const;

public:
	MStatus	ExportScene(const MFileObject& mfoM2File,const MString& msExportOptions, MPxFileTranslator::FileAccessMode mode);
	MStatus translateNode(MDagPath& dagPath,feibian *pSkeleton);
	void loadMtlKeyframes();

	Mesh	 *m_pMesh;
	MaterialSet *m_pMaterialSet;
};
