#include "ExportOptions.h"

ExportOptions::ExportOptions(void)
{
	exportAll = true;
	exportMesh = true;
	exportVertexColour = false;
	exportMaterials = true;
	copyTextures = true;
	exportSkeleton = true;
	exportAnims = true;
	exportAllFrame = true;
	clipList.clear();
	frameIndex = 0;
	deleteUnReferrencedBones = false;
}

ExportOptions* GetExportOptions()
{
	return &tExportOptions;
}
