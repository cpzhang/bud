#include "Group.h"
#include "tinyxml2/tinyxml2.h"
#include "Entity.h"
Group::Group()
{
	_clear();
}
Group::~Group()
{

}
void Group::decode(const std::string& fileName)
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
	tinyxml2::XMLElement* mes = r->FirstChildElement("entity");
	if (NULL == mes)
	{
		return;
	}
	while(mes)
	{
		Entity* e = new Entity;
		std::string path = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
		path = Buddha::FileSystem::getInstancePtr()->getParent(path);
		path += "/";
		e->decode(path + mes->Attribute("file"));
		e->mType = mes->Attribute("type");
		mEntities.push_back(e);
		mes = mes->NextSiblingElement();
	}
}
void Group::render()
{
	for(size_t i = 0; i != mEntities.size(); ++i)
	{
		mEntities[i]->render();
	}
}
void Group::destroy()
{
	for(size_t i = 0; i != mEntities.size(); ++i)
	{
		mEntities[i]->destroy();
		delete mEntities[i];
		mEntities[i] = NULL;
	}
	_clear();
}
void Group::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	for(size_t i = 0; i != mEntities.size(); ++i)
	{
		if (NULL == mEntities[i])
		{
			continue;
		}
		mEntities[i]->setMatrix(name, pMatrix);
	}
}
void Group::_clear()
{
	mName.clear();
	mEntities.clear();
}

AniMapIterator Group::getAniMapIterator( void ) const
{
	return mEntities[0]->getAniMapIterator();
}

void Group::setAnimationName( const std::string& fileName )
{
	for(size_t i = 0; i != mEntities.size(); ++i)
	{
		mEntities[i]->setAnimationName(fileName);
	}
}

void Group::update( u32 delta )
{
	for(size_t i = 0; i != mEntities.size(); ++i)
	{
		mEntities[i]->update(delta);
	}
}

bool Group::intersect( const Ray& r, Real& p )
{
	for (size_t i = 0; i != mEntities.size(); ++i)
	{
		bool b = mEntities[i]->intersect(r, p);
		if (b)
		{
			return b;
		}
	}
	return false;
}
