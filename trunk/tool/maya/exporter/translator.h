#pragma once
#include "ExportOptions.h"
#include "writer.h"

class translator : public MPxFileTranslator
{
public:
	translator(){}
	virtual ~translator(void);
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

private:
	MWriter m_mWriter;
};
