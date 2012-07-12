#include "MzModel.h"
#include "render/Euclid.h"
#include "Geometry.h"
MzModel::MzModel()
{
	_clear();
}
MzModel::~MzModel()
{

}
void MzModel::saveFile(const std::string& fileName)
{
	Euclid::MZ::saveAnimation(fileName);
}

void MzModel::decode(const std::string& fileName)
{
	std::string exportPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
	std::string fileFinalName = Buddha::FileSystem::getInstancePtr()->removeParent(fileName);
	fileFinalName = Buddha::FileSystem::getInstancePtr()->removeFileExtension(fileFinalName);
	exportPath += "/mzNew/";
	exportPath += fileFinalName;
	Euclid::MZ::load(fileName);
	Euclid::Mesh m;
	std::string skeletonPath;
	std::string skinPath;
	for (int i = 0; i < Euclid::MZ::getSubMeshNumber(); ++i)
	{
		Euclid::MZ::create(i, &m);
		std::string path = exportPath + "/mesh/" + Euclid::MZ::getSubMeshName(i) + ".mesh";
		Buddha::FileSystem::getInstancePtr()->createFolder(path);
		m.save(path);
		//
		if(i == 0){
			skeletonPath = exportPath + "/skeleton/" + Euclid::MZ::getSubMeshName(i) + ".skeleton";
			Buddha::FileSystem::getInstancePtr()->createFolder(skeletonPath);
			Euclid::MZ::saveSkeleton(skeletonPath);
		}
		std::string bmPath = exportPath + "/mesh/" + Euclid::MZ::getSubMeshName(i) + ".boneMapping";
		Buddha::FileSystem::getInstancePtr()->createFolder(bmPath);
		m.saveBoneMapping(bmPath);

		m.destroy();
		Geometry* g = new Geometry;
		g->decode(path);
		mGeometries.push_back(g);
		
		g->setSkeleton(skeletonPath);
		//
		g->setSkeletonMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
		//
		if (i == 0)
		{
			skinPath = exportPath + "/skin/";
			Buddha::FileSystem::getInstancePtr()->createFolder(skinPath);
			Euclid::MZ::saveSkin(skinPath);
		}
		
		//
		g->setSkin(skinPath + "Stand.skin");
		//
		g->setBoneMapping(exportPath + "/mesh/" 
			+ Euclid::MZ::getSubMeshName(i) + ".boneMapping");
	}
}
void MzModel::render()
{
	for (size_t i = 0; i != mGeometries.size(); ++i)
	{
		mGeometries[i]->render();
	}
}
void MzModel::destroy()
{
	for (size_t i = 0; i != mGeometries.size(); ++i)
	{
		mGeometries[i]->destroy();
		delete mGeometries[i];
	}
	_clear();
}
void MzModel::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	for (size_t i = 0; i != mGeometries.size(); ++i)
	{
		mGeometries[i]->setMatrix(name, pMatrix);
	}
}
void MzModel::update(u32 delta)
{
	for (size_t i = 0; i != mGeometries.size(); ++i)
	{
		mGeometries[i]->update(delta);
	}
}

void MzModel::_clear()
{
	mGeometries.clear();
}
