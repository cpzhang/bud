#include "RocketDescriptor.h"
#include "RocketExporter.h"

RocketDescriptor::RocketDescriptor()
{

}

RocketDescriptor::~RocketDescriptor()
{

}

int RocketDescriptor::IsPublic()
{
	return 1;
}

void* RocketDescriptor::Create( BOOL loading /*= FALSE*/ )
{
	return new RocketExporter;
}

const TCHAR* RocketDescriptor::ClassName()
{
	return RocketExporter::tName.c_str();
}

SClass_ID RocketDescriptor::SuperClassID()
{
	return RocketExporter::tSID;
}

Class_ID RocketDescriptor::ClassID()
{
	return RocketExporter::tCID;
}

const TCHAR* RocketDescriptor::Category()
{
	return RocketExporter::tCategory.c_str();
}

RocketDescriptor* GetRocketDescriptor()
{
	return &tRocketDescriptor;
}
