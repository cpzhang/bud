#include "MzModel.h"
#include "render/Euclid.h"
#include "Entity.h"
#include "tinyXML2/tinyxml2.h"
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
		if(i == 0)
		{
			skeletonPath = exportPath + "/skeleton/" + fileFinalName + ".skeleton";
			Buddha::FileSystem::getInstancePtr()->createFolder(skeletonPath);
			Euclid::MZ::saveSkeleton(skeletonPath);
			//
			std::string materialPath = exportPath + "/material/";
			Buddha::FileSystem::getInstancePtr()->createFolder(materialPath);
			materialPath = Buddha::FileSystem::getInstancePtr()->standardFilePath(materialPath);
			Euclid::MZ::saveMaterial(materialPath);
			//
			{
				std::string animationsPath = exportPath + "/animation/";
				Buddha::FileSystem::getInstancePtr()->createFolder(animationsPath);
				animationsPath += fileFinalName;
				animationsPath += ".animation";
				animationsPath = Buddha::FileSystem::getInstancePtr()->standardFilePath(animationsPath);
				Euclid::MZ::saveAnimation(animationsPath);
			}
			//
			{
				std::string animationsPath = exportPath + "/subEntity/";
				Buddha::FileSystem::getInstancePtr()->createFolder(animationsPath);
				Euclid::MZ::saveSubEntity(animationsPath);
			}
			//
			{
				skinPath = exportPath + "/skin/";
				Buddha::FileSystem::getInstancePtr()->createFolder(skinPath);
				Euclid::MZ::saveSkin(skinPath);
			}
		}
		std::string bmPath = exportPath + "/mesh/" + Euclid::MZ::getSubMeshName(i) + ".boneMapping";
		Buddha::FileSystem::getInstancePtr()->createFolder(bmPath);
		m.saveBoneMapping(bmPath);

		m.destroy();		
	}
	// .entity 
	{
		//============================================================================
		tinyxml2::XMLDocument doc;
		// 
		tinyxml2::XMLDeclaration* dec = doc.NewDeclaration("xml version=\"1.0\"");
		doc.LinkEndChild(dec);
		//
		tinyxml2::XMLElement* ele = doc.NewElement("entity");
		ele->SetAttribute("name", fileFinalName.c_str());

		for (int i = 0; i < Euclid::MZ::getSubMeshNumber(); ++i)
		{
			tinyxml2::XMLElement* a = doc.NewElement("subEntity");
			std::string meshPath;
			meshPath = "subEntity/";
			meshPath += Euclid::MZ::getSubMeshName(i);
			meshPath += ".subEntity";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}
		
		doc.LinkEndChild(ele);
		//
		std::string path = exportPath + "/entity/" + fileFinalName + ".entity";
		Buddha::FileSystem::getInstancePtr()->createFolder(path);
		doc.SaveFile(path.c_str());
		//
		{
			mEntity = new Entity;
			mEntity->decode(path);
		}
	}
}
void MzModel::render()
{
	mEntity->render();
}
void MzModel::destroy()
{
	mEntity->destroy();
	delete mEntity;
	_clear();
}
void MzModel::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	mEntity->setMatrix(name, pMatrix);
}

void MzModel::update(u32 delta)
{
	mEntity->update(delta);
}


void MzModel::_clear()
{
	mEntity = NULL;
}

AniMapIterator MzModel::getAniMapIterator( void ) const
{
	return mEntity->getAniMapIterator();
}

void MzModel::setAnimationName( const std::string& fileName )
{
	mEntity->setAnimationName(fileName);
}
