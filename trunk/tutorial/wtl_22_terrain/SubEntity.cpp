#include "SubEntity.h"
#include "tinyxml2/tinyxml2.h"
#include "Geometry.h"
SubEntity::SubEntity()
{
	_clear();
}

SubEntity::~SubEntity()
{

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
void SubEntity::render()
{
	if (mGeometry)
	{
		mGeometry->render();
	}
}

void SubEntity::update(u32 delta)
{
	mGeometry->update(delta);
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
void SubEntity::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	if (mGeometry)
	{
		mGeometry->setMatrix(name, pMatrix);
	}
}

void SubEntity::_clear()
{
	mName.clear();
	mGeometry = NULL;
	mAnimations.clear();
}

AniMapIterator SubEntity::getAniMapIterator( void ) const
{
	return mGeometry->getAniMapIterator();
}

void SubEntity::setAnimationName( const std::string& fileName )
{
	mGeometry->setAnimationName(fileName);
}

void SubEntity::stopAnimation()
{
	mGeometry->stopAnimation();
}

void SubEntity::playAnimation()
{
	mGeometry->playAnimation();
}

void SubEntity::setSpeed( float speed )
{
	mGeometry->setSpeed(speed);
}
