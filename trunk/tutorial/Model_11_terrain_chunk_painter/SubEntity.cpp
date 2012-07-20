#include "SubEntity.h"
#include "Geometry.h"
#include "tinyxml2/tinyxml2.h"
SubEntity::SubEntity()
	{
		_clear();
	}
	SubEntity::~SubEntity()
	{

	}

	void SubEntity::_clear()
	{
		mType = eVisibleObjectType_SubEntity;
		mName.clear();
		mGeometry = NULL;
		mAnimations.clear();
	}

void SubEntity::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	if (mGeometry)
	{
		mGeometry->setMatrix(name, pMatrix);
	}
}

void SubEntity::destroy()
{
	if (mGeometry)
	{
		mGeometry->destroy();
		delete mGeometry;
	}
	_clear();
}

void SubEntity::render()
{
	if (mGeometry)
	{
		mGeometry->render();
	}
}
void SubEntity::decode(const std::string& fileName)
{
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return;
	}
	tinyxml2::XMLElement* r = doc.RootElement();
	if (NULL == r)
	{
		return;
	}
	std::string parentPath = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
	parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
	parentPath += "/";
	mName = r->Attribute("name");
	{
		tinyxml2::XMLElement* mes = r->FirstChildElement("mesh");
		mGeometry = new Geometry;
		mGeometry->setMesh(parentPath + mes->Attribute("file"));
	}

	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("material");
		mGeometry->setMaterial(parentPath + mat->Attribute("file"));
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			mGeometry->setSkeleton(parentPath + skeleton);
		}
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("boneMapping");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			mGeometry->setBoneMapping(parentPath + skeleton);
		}
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("animation");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			mGeometry->setAnimation(parentPath + skeleton);
		}
	}
}
void SubEntity::update(u32 delta)
{
	mGeometry->update(delta);
}