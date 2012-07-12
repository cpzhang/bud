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
	
	mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
	mMaterial->setEffect(shaderFileName);
	mMaterial->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureBoneWeightColor);
	std::string imagePath = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
	imagePath = Buddha::FileSystem::getInstancePtr()->getParent(imagePath);
	imagePath += "/";
	mMaterial->setTexture(textureVariableName, imagePath + textureFileName);
	if (r->Attribute("zEnable"))
	{
		mMaterial->_zEnable = (Euclid::eZBufferType)r->IntAttribute("zEnable");
	}
	if (r->Attribute("zWriteEnable"))
	mMaterial->_zWriteEnable = r->BoolAttribute("zWriteEnable");

	if (r->Attribute("alphaTestEnable"))
	mMaterial->_alphaTestEnable = r->BoolAttribute("alphaTestEnable");

	if (r->Attribute("srcBlend"))
	mMaterial->_srcBlend = (Euclid::eBlend)r->IntAttribute("srcBlend");

	if (r->Attribute("destBlend"))
	mMaterial->_destBlend = (Euclid::eBlend)r->IntAttribute("destBlend");

	if (r->Attribute("cmpFunc"))
	mMaterial->mAlphaCmpFunc = (Euclid::eCmpFunc)r->IntAttribute("cmpFunc");

	if (r->Attribute("alphaRef"))
	mMaterial->mAlphaRef = r->IntAttribute("alphaRef");

	if (r->Attribute("diffuse"))
	mMaterial->mDiffuse.setRGBA(r->IntAttribute("diffuse"));

	if (r->Attribute("fillMode"))
		mMaterial->mFillMode = Euclid::eFillMode(r->IntAttribute("fillMode"));

	if (r->Attribute("useVertexColor"))
	mMaterial->mUseVertexColor = r->BoolAttribute("useVertexColor");
	//
	{
		std::string aniPath = Buddha::FileSystem::getInstancePtr()->removeFileExtension(fileName) + ".ma";
		mMaterial->loadAnimation(aniPath);
	}
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
	std::string filetype = Buddha::FileSystem::getInstancePtr()->getFileExtension(fileName);
	if (filetype == "terrain")
	{
		Euclid::Terrain t;
		t.create(fileName);
		mMesh->createFromTerrain(&t);
	} 
	else if(filetype == "mesh")
	{
		mMesh->create(mMeshFile);
	}
	else if(filetype == "raw")
	{
		mMesh->createFromHeightMap(mMeshFile, 129, 129);
	}
	else
	{
		Error("no such file");
	}
	//
	mAABB.update(mMesh->getAABB());
}
void Geometry::setSkeleton(const std::string& fileName)
{
	mSkeleton = new Euclid::Skeleton;
	mSkeleton->create(fileName);
}
void Geometry::update(u32 delta)
{
	if (mAniTime.end <= mAniTime.start)
	{
		return;
	}
	char szPath[MAX_PATH] = {0};
	char szPath2[MAX_PATH];
	while(mAniTime.current > mAniTime.end)
	{
		mAniTime.current -= mAniTime.end;
	}
	if (mSkin && mSkeleton)
	{
		mSkeleton->update(mAniTime, *mSkin);
	}
	if (!mStopAimation)
	{
		mAniTime.current += delta * m_speed;;
	}
	//
	Vec3 speed(0.000, 0.000, 0.0);
	Quaternion q(0, 0, 0, 0.000*mAniTime.current);
	Mat4 tQ(q);
	Mat4 tT = Mat4::IDENTITY;
	Vec3 offsetMatrix(-0.5, -0.5, 0.0);
	tT.setTrans(offsetMatrix);
	mUVMatrix = tT.inverse() * tQ * tT;
	//
	mMaterial->update(delta);
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
		if (matrices.size() >= 60 || matrices.empty())
		{
			mHardwareSkin = false;
		}
		else
		{
			fx->setMatrixArray("gMatrixPalette", &matrices[0], matrices.size()); 
			mHardwareSkin = true;
		}
		fx->setBool("gSkinned", mHardwareSkin);
	}
	
	fx->setMatrix("gUVMatrix", &mUVMatrix);
	float alpha = mMaterial->mAlphaKFs.getFrame(&mMaterial->mAlphaAnimationTime);
	mMaterial->mDiffuse.a *= alpha;
	Euclid::AnimationTime at;
	Euclid::Color3 c = mMaterial->mColorKFs.getFrame(&at);
	mMaterial->mDiffuse.r *= c.r;
	mMaterial->mDiffuse.g *= c.g;
	mMaterial->mDiffuse.b *= c.b;
	fx->setFloatArray("gDiffuse", &mMaterial->mDiffuse.r, 4);
	fx->setBool("gUseVertexColor", mMaterial->mUseVertexColor);
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
	//
	renderAABB();
	//
	//_renderIntersection();
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
	if (mMaterialPW)
	{
		mMaterialPW->destroy();
		delete mMaterialPW;
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
	if (mMaterial && mMaterial->getEffect())
	{
		mMaterial->getEffect()->setMatrix(name, pMatrix);
	}
	//
	if (mSkeletonMaterial && mSkeletonMaterial->getEffect())
	{
		mSkeletonMaterial->getEffect()->setMatrix(name, pMatrix);
	}
	//
	if (mMaterialPW && mMaterialPW->getEffect())
	{
		mMaterialPW->getEffect()->setMatrix(name, pMatrix);
	}
	//
	mAABB.setMatrix(name, pMatrix);
}
void Geometry::_clear()
{
	mName.clear();
	mAnimationName.clear();
	mMeshFile.clear();
	mMesh = NULL;
	mMaterial = NULL;
	mSkeleton = NULL;
	mSkeletonMaterial = NULL;
	mSkin = NULL;
	mUVMatrix = Mat4::IDENTITY;
	mStopAimation = false;
	mMaterialPW = NULL;
//	m_speed = 1.0f;
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
	if (fileName == mAnimationName)
	{
		return;
	}
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
	mAnimationName = fileName;
}

void Geometry::setSpeed( float speed )
{
	m_speed = speed;
}

void Geometry::_renderAABB()
{
	if (NULL == mMesh)
	{
		return;
	}
	if (NULL == mMaterialPW)
	{
		mMaterialPW = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		mMaterialPW->setEffect("shader/aP.fx");
		mMaterialPW->setVertexDeclaration(Euclid::eVertexDeclarationType_Position);
		mMaterialPW->mFillMode = Euclid::eFillMode_WireFrame;
	}

	Euclid::Effect* fx = mMaterialPW->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mMaterialPW->apply();

	//
	Euler::AABB* ab = mMesh->getAABB();
	//
	Euclid::sPosition p[8];
	p[0].position = ab->_max;
	p[1].position = Vec3(ab->_max.x, ab->_max.y, ab->_min.z);
	p[2].position = Vec3(ab->_min.x, ab->_max.y, ab->_min.z);
	p[3].position = Vec3(ab->_min.x, ab->_max.y, ab->_max.z);

	p[4 + 0].position = Vec3(ab->_max.x, ab->_min.y, ab->_max.z);
	p[4 + 1].position = Vec3(ab->_max.x, ab->_min.y, ab->_min.z);
	p[4 + 2].position = ab->_min;
	p[4 + 3].position = Vec3(ab->_min.x, ab->_min.y, ab->_max.z);

	u16 index[12 * 3] = 
	{
		//上
		0, 1, 2, 
		0, 2, 3,

		//下
		4 + 0, 4 + 1, 4 + 2,
		4 + 0, 4 + 2, 4 + 3,

		//前
		4 + 0, 0, 3,
		4 + 0, 3, 4 + 3,

		//后
		4 + 1, 1, 2,
		4 + 1, 2, 4 + 2,

		//左
		4 + 3, 3, 2,
		4 + 3, 2, 4 + 2,

		//右
		4 + 1, 1, 0,
		4 + 1, 0, 4 + 0,
	};
	//
	{
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawIndexedPrimitiveUP(Euclid::ePrimitive_TriangleList, 0, 8, 12, &index[0], Euclid::eFormat_Index16, &p[0], sizeof(Euclid::sPosition));

			fx->endPass();
		}
		fx->end();
	}
}

bool Geometry::intersect( const Ray& r, Real& p )
{
	if (mMesh)
	{
		return mMesh->intersect(r, p);
	}
	return false;
}

void Geometry::_renderIntersection()
{
	if (mSkeletonMaterial == NULL)
	{
		setSkeletonMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
	}
	Euclid::Effect* fx = mSkeletonMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mSkeletonMaterial->apply();

	//	
	{
		Euclid::sPosition& p = RenderEngineImp::getInstancePtr()->mIntersection;
		u32 passes = 0;
		Euclid::Color c = Euclid::Color::Red;
		fx->setFloatArray("gDiffuse", &c.r, 4);
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			//
			{

				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 1, &p.position[0], sizeof(Euclid::sPosition));
			}

			fx->endPass();
		}
		fx->end();
	}
	//	
	{
		Euclid::sPosition& p = RenderEngineImp::getInstancePtr()->mIntersectionClicked;
		u32 passes = 0;
		Euclid::Color c = Euclid::Color::Green;
		fx->setFloatArray("gDiffuse", &c.r, 4);
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			//
			{

				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 1, &p.position[0], sizeof(Euclid::sPosition));
			}

			fx->endPass();
		}
		fx->end();
	}			
}

void Geometry::geomipmapping( u32 lod )
{
	if (NULL == mMesh)
	{
		return;
	}
	mMesh->geomipmapping(lod);
}

float Geometry::m_speed(1.0f);
