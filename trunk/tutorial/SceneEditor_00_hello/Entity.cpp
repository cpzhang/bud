#include "Entity.h"
#include "tinyxml2/tinyxml2.h"
#include "SubEntity.h"
Entity::Entity()
{
	_clear();
}

Entity::~Entity()
{

}
void Entity::decode(const std::string& fileName)
{
	//
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return;
	}
	tinyxml2::XMLElement* r = doc.RootElement();

	tinyxml2::XMLElement* tex= r->FirstChildElement("subEntity");

	std::string parentPath = Buddha::FileSystem::getInstancePtr()->standardFilePath(fileName);
	parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
	parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
	parentPath += "/";
	while (tex)
	{
		std::string subEntityFileName = tex->Attribute("file");
		SubEntity* sub = new SubEntity;
		sub->decode(parentPath + subEntityFileName);
		mSubEntities.push_back(sub);
		tex = tex->NextSiblingElement("subEntity");
	}

	
}
void Entity::render()
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->render();
	}
}

void Entity::update(u32 delta)
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->update(delta);
	}
}

void Entity::destroy()
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->destroy();
		delete mSubEntities[i];
	}
	_clear();
}
void Entity::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->setMatrix(name, pMatrix);
	}
}

void Entity::_clear()
{
	mName.clear();
	mSubEntities.clear();
}

AniMapIterator Entity::getAniMapIterator( void ) const
{
	return mSubEntities[0]->getAniMapIterator();
}

void Entity::setAnimationName( const std::string& fileName )
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->setAnimationName(fileName);
	}
}

void Entity::stopAnimation()
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->stopAnimation();
	}
}

void Entity::playAnimation()
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->playAnimation();
	}
}

void Entity::setSpeed( float speed )
{
	for (size_t i = 0; i != mSubEntities.size(); ++i)
	{
		mSubEntities[i]->setSpeed(speed);
	}
}
