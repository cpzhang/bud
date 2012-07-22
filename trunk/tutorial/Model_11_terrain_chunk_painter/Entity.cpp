#include "Entity.h"
#include "SubEntity.h"
#include "tinyxml2/tinyxml2.h"
Entity::Entity()
{
  _clear();
}
Entity::~Entity()
{

}
void Entity::_clear()
{
  mType = eVisibleObjectType_Entity;
  mName.clear();
  mSubEntities.clear();
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
  mName = r->Attribute("name");
  tinyxml2::XMLElement* tex= r->FirstChildElement("subEntity");

  std::string parentPath = Zen::FileSystem::getInstancePtr()->standardFilePath(fileName);
  parentPath = Zen::FileSystem::getInstancePtr()->getParent(parentPath);
  parentPath = Zen::FileSystem::getInstancePtr()->getParent(parentPath);
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
