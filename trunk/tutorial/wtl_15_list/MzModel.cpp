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
	Euclid::MZ::load(fileName);
	Euclid::Mesh m;
	std::string skeletonPath;
	std::string skinPath;
	for (int i = 0; i < Euclid::MZ::getSubMeshNumber(); ++i)
	{
		Euclid::MZ::create(i, &m);
		std::string path = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "mesh/" + Euclid::MZ::getSubMeshName(i) + ".mesh";
		m.save(path);
		m.destroy();
		Geometry* g = new Geometry;
		g->decode(path);
		mGeometries.push_back(g);
		//
		if (i == 0)
		{
			skeletonPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "skin/" + Euclid::MZ::getSubMeshName(i) + ".skeleton";
			Euclid::MZ::saveSkeleton(skeletonPath);
		}
		
		g->setSkeleton(skeletonPath);
		//
		g->setSkeletonMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
		//
		if (i == 0)
		{
			skinPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "skin/";
			Euclid::MZ::saveSkin(skinPath);
		}
		
		//
		g->setSkin(skinPath + "dead.skin");
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
