#include "Geometry.h"
#include "global.h"
#include "tinyxml2/tinyxml2.h"
Geometry::Geometry()
{
	_clear();
}
Geometry::~Geometry()
{

}
void Geometry::setMaterial(const std::string& fileName)
{
	std::string name;
	std::string shaderFileName;
	std::string textureVariableName;
	std::string textureFileName;
	//
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
		name = r->Attribute("name");
		shaderFileName = r->Attribute("shader");
		//
		{
			tinyxml2::XMLElement* mat = r->FirstChildElement("texture");
			textureVariableName = mat->Attribute("name");
			textureFileName = mat->Attribute("file");
		}
	}
	mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
	mMaterial->setEffect(shaderFileName);
	mMaterial->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureBoneWeight);
	std::string imagePath = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
	imagePath = Buddha::FileSystem::getInstancePtr()->getParent(imagePath);
	imagePath += "/";
	mMaterial->setTexture(textureVariableName, imagePath + textureFileName);
}
void Geometry::setSkeletonMaterial(const std::string& fileName, Euclid::eMaterialType mt)
{
	mSkeletonMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(mt);
	mSkeletonMaterial->setEffect(fileName);
}
void Geometry::setSkin(const std::string& fileName)
{
	if (mSkin)
	{
		delete mSkin;
		mSkin = NULL;
	}
	mSkin = new Euclid::Skin;
	mSkin->create(fileName);
}
Euclid::IMaterial* Geometry::getMaterial()
{
	return mMaterial;
}
void Geometry::setMesh(const std::string& fileName)
{
	mMeshFile = fileName;
	mMesh = new Euclid::Mesh;
	mMesh->create(mMeshFile);
	//
}
void Geometry::setSkeleton(const std::string& fileName)
{
	mSkeleton = new Euclid::Skeleton;
	mSkeleton->create(fileName);
}
void Geometry::update(u32 delta)
{
	while(mAniTime.current > mAniTime.end)
	{
		mAniTime.current -= mAniTime.end;
	}
	if (mSkin && mSkeleton)
	{
		mSkeleton->update(mAniTime, *mSkin);
	}
	mAniTime.current += delta;
}
void Geometry::decode(const std::string& fileName)
{
	//
	setMesh(fileName);
	//
	//setMaterial("shader/aP.fx");
}
void Geometry::_renderMesh()
{
	if (NULL == mMesh)
	{
		return;
	}
	if (NULL == mMaterial)
	{
		return;
	}
	
	Euclid::Effect* fx = mMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mMaterial->apply();
	
	//
	if (NULL != mSkeleton)
	{
		//
		std::vector<Mat4> matrices;
		for(Euclid::BoneIDReferenceMap::iterator it = mMesh->_bones.begin(); it != mMesh->_bones.end(); ++it)
		{
			if (it->first < 255)
			{
				matrices.push_back(mSkeleton->_matrices[it->first]);
			}
		}
		if (matrices.size() >= 60)
		{
			mHardwareSkin = false;
		}
		else
		{
			mHardwareSkin = true;
		}
		fx->setMatrixArray("gMatrixPalette", &matrices[0], matrices.size()); 
	}
	
	u32 passes = 0;
	fx->begin(&passes);
	for (u32 i = 0; i != passes; ++i)
	{
		fx->beginPass(i);
		mMesh->render();
		fx->endPass();
	}
	fx->end();
}
void Geometry::render()
{
	_renderMesh();
	//
	renderSkeleton();
}
void Geometry::renderSkeleton()
{
	if (NULL == mSkeleton)
	{
		return;
	}
	if (NULL == mSkeletonMaterial)
	{
		return;
	}
	Euclid::Effect* fx = mSkeletonMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mSkeletonMaterial->apply();

	Euclid::BoneNodeMapIterator it = mSkeleton->getCommandMapIterator();
	while(!it.isAtEnd())
	{
		Euclid::BoneNode* n = it.getCurrentValue();
		Euclid::BoneNode* p = n->getParentNode();
		if (NULL != p)
		{
			Euclid::Bone* b = n->getBone();
			Euclid::Bone* f = p->getBone();
			//
			Euclid::sPosition p[2];
			p[0].position = mSkeleton->_matricesFull[b->id] * p[0].position;
			p[1].position = mSkeleton->_matricesFull[f->id] * p[1].position;

			//
			{
				u32 passes = 0;
				fx->begin(&passes);
				for (u32 i = 0; i != passes; ++i)
				{
					fx->beginPass(i);

					RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_LineList, 1, p, sizeof(Euclid::sPosition));
					//
					RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 2, p, sizeof(Euclid::sPosition));

					fx->endPass();
				}
				fx->end();
			}
		}
		//
		++it;
	}
	//
	it.toStart();
	//
	Mat4 vm, pm;
	fx->getMatrix("gView", &vm);
	fx->getMatrix("gProjection", &pm);
	while(!it.isAtEnd())
	{
		Euclid::BoneNode* n = it.getCurrentValue();
		{
			Euclid::Bone* b = n->getBone();
			//
			Vec3 p;
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->objectSpace2ScreenSpace(&p, &p, NULL, &pm, &vm, &mSkeleton->_matricesFull[b->id]);
			//
			{
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont("freeNormal")->render(p, Vec3(1, 0, 0), Euclid::Color::Red, b->name);
			}
		}
		//
		++it;
	}
}
void Geometry::destroy()
{
	if (mMesh)
	{
		mMesh->destroy();
		delete mMesh;
	}
	if (mMaterial)
	{
		mMaterial->destroy();
		delete mMaterial;
	}
	if (mSkeletonMaterial)
	{
		mSkeletonMaterial->destroy();
		delete mSkeletonMaterial;
	}
	if (mSkeleton)
	{
		mSkeleton->destroy();
		delete mSkeleton;
	}
	if (mSkin)
	{
		delete mSkin;
	}
	_clear();
}
void Geometry::setMatrix( const std::string& name, const Mat4& pMatrix )
{
	if (NULL == mMesh)
	{
		return;
	}
	if (NULL == mMaterial)
	{
		return;
	}
	Euclid::Effect* fx = mMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	fx->setMatrix(name, pMatrix);
	//
	{
		if (NULL == mSkeletonMaterial)
		{
			return;
		}
		Euclid::Effect* fx = mSkeletonMaterial->getEffect();
		if (NULL == fx)
		{
			return;
		}
		fx->setMatrix(name, pMatrix);
	}
}
void Geometry::_clear()
{
	mName.clear();
	mMeshFile.clear();
	mMesh = NULL;
	mMaterial = NULL;
	mSkeleton = NULL;
	mSkeletonMaterial = NULL;
	mSkin = NULL;
}

void Geometry::setBoneMapping( const std::string& fileName )
{
	if (mMesh)
	{
		mMesh->loadBoneMapping(fileName);
	}
}

void Geometry::setAnimation( const std::string& fileName )
{
	//
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
	{
		return;
	}
	tinyxml2::XMLElement* r = doc.RootElement();

	tinyxml2::XMLElement* tex= r->FirstChildElement("animation");
		
	while (tex)
	{
		sAnimation an;
		an.name = tex->Attribute("name");
		an.skinFile = tex->Attribute("skin");
		an.duration = tex->IntAttribute("duration");
		mAnimations[an.name] = an;

		tex = tex->NextSiblingElement("animation");
	}

	//
	std::string parentPath = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
	parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
	setSkin(parentPath + "/" + mAnimations.begin()->second.skinFile);
	mAniTime.current = 0;
	mAniTime.start = 0;
	mAniTime.end = mAnimations.begin()->second.duration;
}

AniMapIterator Geometry::getAniMapIterator( void ) const
{
	return AniMapIterator(mAnimations.begin(), mAnimations.end());
}

void Geometry::setAnimationName( const std::string& fileName )
{
	if (mSkin)
	{
		delete mSkin;
		mSkin = NULL;
	}
	//
	std::string parentPath = Buddha::FileSystem::getInstancePtr()->getParent(mMeshFile);
	parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
	parentPath += "/";
	AniMap::iterator it = mAnimations.find(fileName.c_str());
	if (it == mAnimations.end())
	{
		return;
	}
	mAniTime.current = 0;
	mAniTime.start = 0;
	mAniTime.end = it->second.duration;
	std::string skinFile = it->second.skinFile.c_str();
	parentPath += skinFile;
	setSkin(parentPath);
}
