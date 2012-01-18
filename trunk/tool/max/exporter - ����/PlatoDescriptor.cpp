#include "PlatoDescriptor.h"
#include "PlatoExporter.h"

PlatoDescriptor::PlatoDescriptor()
{

}

PlatoDescriptor::~PlatoDescriptor()
{

}

int PlatoDescriptor::IsPublic()
{
	return 1;
}

void* PlatoDescriptor::Create( BOOL loading /*= FALSE*/ )
{
	return new PlatoExporter;
}

const TCHAR* PlatoDescriptor::ClassName()
{
	return PlatoExporter::tName.c_str();
}

SClass_ID PlatoDescriptor::SuperClassID()
{
	return PlatoExporter::tSID;
}

Class_ID PlatoDescriptor::ClassID()
{
	return PlatoExporter::tCID;
}

const TCHAR* PlatoDescriptor::Category()
{
	return PlatoExporter::tCategory.c_str();
}

PlatoDescriptor* GetPlatoDescriptor()
{
	return &tPlatoDescriptor;
}
