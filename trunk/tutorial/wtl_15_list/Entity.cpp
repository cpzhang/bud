#include "Entity.h"
#include "tinyxml2/tinyxml2.h"
#include "Geometry.h"
Entity::Entity()
{
	_clear();
}

Entity::~Entity()
{

}
void Entity::decode(const std::string& fileName)
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
	mName = r->Attribute("name");
	tinyxml2::XMLElement* mes = r->FirstChildElement("mesh");
	if (NULL == mes)
	{
		return;
	}

	mGeometry = new Geometry;
	mGeometry->setMesh(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + mes->Attribute("file"));
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("material");
		if (mat)
		{
			mGeometry->setMaterial(mat->Attribute("shader"), Euclid::eMaterialType_VertexTexture);
			tinyxml2::XMLElement* tex = mat->FirstChildElement("texture");
			if (tex)
			{
				mGeometry->getMaterial()->setTexture(tex->Attribute("name"), tex->Attribute("value"));
			}
		}
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("animations");
		if (mat)
		{
			tinyxml2::XMLElement* tex = mat->FirstChildElement("animation");
			if (tex)
			{
				sAnimation an;
				an.name = tex->Attribute("name");
				an.skinFile = tex->Attribute("skin");
				an.duration = tex->IntAttribute("duration");
				mAnimations[an.name] = an;
			}
		}
		//
		mGeometry->setSkin(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + mAnimations.begin()->second.skinFile);
	}
	//
	{
		tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
		if (mat)
		{
			std::string skeleton = mat->Attribute("file");
			mGeometry->setSkeleton(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + skeleton);
		}
	}
}
void Entity::render()
{
	if (mGeometry)
	{
		mGeometry->render();
	}
}

void Entity::update(u32 delta)
{
	mGeometry->update(delta);
}

void Entity::destroy()
{
	if (mGeometry)
	{
		mGeometry->destroy();
		delete mGeometry;
	}
	_clear();
}
void Entity::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	if (mGeometry)
	{
		mGeometry->setMatrix(name, pMatrix);
	}
}

void Entity::_clear()
{
	mName.clear();
	mGeometry = NULL;
	mAnimations.clear();
}