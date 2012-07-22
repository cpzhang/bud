#include "MzModel.h"
#include "tinyxml2/tinyxml2.h"
#include "Entity.h"
MzModel::MzModel()
{
	_clear();
}
MzModel::~MzModel()
{

}
void MzModel::saveFile(const std::string& fileName)
{
	Zen::MZ::saveAnimation(fileName);
}

void MzModel::decode(const std::string& fileName)
{
	std::string exportPath = Zen::FileSystem::getInstancePtr()->getDataDirectory();
	std::string fileFinalName = Zen::FileSystem::getInstancePtr()->removeParent(fileName);
	fileFinalName = Zen::FileSystem::getInstancePtr()->removeFileExtension(fileFinalName);
	exportPath += "/mzNew/";
	exportPath += fileFinalName;
	Zen::MZ::load(fileName);
	Zen::Mesh m;
	std::string skeletonPath;
	std::string skinPath;
	for (int i = 0; i < Zen::MZ::getSubMeshNumber(); ++i)
	{
		Zen::MZ::create(i, &m);
		std::string path = exportPath + "/mesh/" + Zen::MZ::getSubMeshName(i) + ".mesh";
		Zen::FileSystem::getInstancePtr()->createFolder(path);
		m.save(path);
		//
		if(i == 0)
		{
			skeletonPath = exportPath + "/skeleton/" + fileFinalName + ".skeleton";
			Zen::FileSystem::getInstancePtr()->createFolder(skeletonPath);
			Zen::MZ::saveSkeleton(skeletonPath);
			//
			std::string materialPath = exportPath + "/material/";
			Zen::FileSystem::getInstancePtr()->createFolder(materialPath);
			materialPath = Zen::FileSystem::getInstancePtr()->standardFilePath(materialPath);
			Zen::MZ::saveMaterial(materialPath);
			//
			{
				std::string animationsPath = exportPath + "/animation/";
				Zen::FileSystem::getInstancePtr()->createFolder(animationsPath);
				animationsPath += fileFinalName;
				animationsPath += ".animation";
				animationsPath = Zen::FileSystem::getInstancePtr()->standardFilePath(animationsPath);
				Zen::MZ::saveAnimation(animationsPath);
			}
			//
			{
				std::string animationsPath = exportPath + "/subEntity/";
				Zen::FileSystem::getInstancePtr()->createFolder(animationsPath);
				Zen::MZ::saveSubEntity(animationsPath);
			}
			//
			{
				skinPath = exportPath + "/skin/";
				Zen::FileSystem::getInstancePtr()->createFolder(skinPath);
				Zen::MZ::saveSkin(skinPath);
			}
		}
		std::string bmPath = exportPath + "/mesh/" + Zen::MZ::getSubMeshName(i) + ".boneMapping";
		Zen::FileSystem::getInstancePtr()->createFolder(bmPath);
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

		for (int i = 0; i < Zen::MZ::getSubMeshNumber(); ++i)
		{
			tinyxml2::XMLElement* a = doc.NewElement("subEntity");
			std::string meshPath;
			meshPath = "subEntity/";
			meshPath += Zen::MZ::getSubMeshName(i);
			meshPath += ".subEntity";
			a->SetAttribute("file", meshPath.c_str());
			ele->LinkEndChild(a);
		}

		doc.LinkEndChild(ele);
		//
		std::string path = exportPath + "/entity/" + fileFinalName + ".entity";
		Zen::FileSystem::getInstancePtr()->createFolder(path);
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
