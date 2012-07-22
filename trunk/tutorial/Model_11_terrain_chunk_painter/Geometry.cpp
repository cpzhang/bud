#include "Geometry.h"
#include "tinyxml2/tinyxml2.h"
#include "renderImp.h"
Geometry::Geometry()
{
	_clear();
}
Geometry::~Geometry()
{

}


	void Geometry::setBoneMapping( const std::string& fileName )
{
	if (mMesh)
	{
		mMesh->loadBoneMapping(fileName);
	}
}
void Geometry::setSkin(const std::string& fileName)
{
	if (mSkin)
	{
		delete mSkin;
		mSkin = NULL;
	}
	mSkin = new Zen::Skin;
	mSkin->create(fileName);
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
	std::string parentPath = Zen::FileSystem::getInstancePtr()->getParent(fileName);
	parentPath = Zen::FileSystem::getInstancePtr()->getParent(parentPath);
	if (!mAnimations.empty())
	{
		setSkin(parentPath + "/" + mAnimations.begin()->second.skinFile);
		mAniTime.current = 0;
		mAniTime.start = 0;
		mAniTime.end = mAnimations.begin()->second.duration;
	}
}

void Geometry::setMesh(const std::string& fileName)
{
	mMeshFile = fileName;
	mMesh = new Zen::Mesh;
	std::string filetype = Zen::FileSystem::getInstancePtr()->getFileExtension(fileName);
	if (filetype == "terrain")
	{
		Zen::Terrain t;
		t.create(fileName);
		mMesh->createFromTerrain(&t);
	} 
	else if(filetype == "mesh")
	{
		mMesh->create(mMeshFile);
	}
	else
	{
		Error("no such file");
	}
	//
}
void Geometry::setSkeleton(const std::string& fileName)
{
	mSkeleton = new Zen::Skeleton;
	mSkeleton->create(fileName);
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

	mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Zen::eMaterialType_VertexTexture);
	mMaterial->setEffect(shaderFileName);
	mMaterial->setVertexDeclaration(Zen::eVertexDeclarationType_PositionTextureBoneWeightColor);
	std::string imagePath = Zen::FileSystem::getInstancePtr()->getParent(fileName);
	imagePath = Zen::FileSystem::getInstancePtr()->getParent(imagePath);
	imagePath += "/";
	if (textureFileName.find('.') != std::string::npos)
	{
		mMaterial->setTexture(textureVariableName, imagePath + textureFileName);
	}

	mMaterial->_zEnable = (Zen::eZBufferType)r->IntAttribute("zEnable");
	mMaterial->_zWriteEnable = r->BoolAttribute("zWriteEnable");
	mMaterial->_alphaTestEnable = r->BoolAttribute("alphaTestEnable");
	mMaterial->_srcBlend = (Zen::eBlend)r->IntAttribute("srcBlend");
	mMaterial->_destBlend = (Zen::eBlend)r->IntAttribute("destBlend");
	mMaterial->mAlphaCmpFunc = (Zen::eCmpFunc)r->IntAttribute("cmpFunc");
	mMaterial->mAlphaRef = r->IntAttribute("alphaRef");
	mMaterial->mDiffuse.setRGBA(r->IntAttribute("diffuse"));
	mMaterial->mUseVertexColor = r->BoolAttribute("useVertexColor");
	//
	{
		std::string aniPath = Zen::FileSystem::getInstancePtr()->removeFileExtension(fileName) + ".ma";
		mMaterial->loadAnimation(aniPath);
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
	mUVMatrix = Mat4::IDENTITY;
	mStopAimation = false;
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

	Zen::Effect* fx = mMaterial->getEffect();
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
		for(Zen::BoneIDReferenceMap::iterator it = mMesh->_bones.begin(); it != mMesh->_bones.end(); ++it)
		{
			if (it->first < 255 && it->first < mSkeleton->_matrices.size())
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
	Zen::AnimationTime at;
	Zen::Color3 c = mMaterial->mColorKFs.getFrame(&at);
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
		if (i == 0)
		{
			mMesh->render();
		}
		fx->endPass();
	}
	fx->end();
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
	Zen::Effect* fx = mSkeletonMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	//
	mSkeletonMaterial->apply();

	Zen::BoneNodeMapIterator it = mSkeleton->getCommandMapIterator();
	while(!it.isAtEnd())
	{
		Zen::BoneNode* n = it.getCurrentValue();
		Zen::BoneNode* p = n->getParentNode();
		if (NULL != p)
		{
			Zen::Bone* b = n->getBone();
			Zen::Bone* f = p->getBone();
			//
			Zen::sPosition p[2];
			p[0].position = mSkeleton->_matricesFull[b->id] * p[0].position;
			p[1].position = mSkeleton->_matricesFull[f->id] * p[1].position;

			//
			{
				u32 passes = 0;
				fx->begin(&passes);
				for (u32 i = 0; i != passes; ++i)
				{
					fx->beginPass(i);

					RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Zen::ePrimitive_LineList, 1, p, sizeof(Zen::sPosition));
					//
					RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Zen::ePrimitive_PointList, 2, p, sizeof(Zen::sPosition));

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
		Zen::BoneNode* n = it.getCurrentValue();
		{
			Zen::Bone* b = n->getBone();
			//
			Vec3 p;
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->objectSpace2ScreenSpace(&p, &p, NULL, &pm, &vm, &mSkeleton->_matricesFull[b->id]);
			//
			{
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont("freeNormal")->render(p, Vec3(1, 0, 0), Zen::Color::Red, b->name);
			}
		}
		//
		++it;
	}
}

void Geometry::decode(const std::string& fileName)
{
//
setMesh(fileName);
//
//setMaterial("shader/aP.fx");
}
void Geometry::render()
{
_renderMesh();
//
renderSkeleton();
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
Zen::Effect* fx = mMaterial->getEffect();
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
	Zen::Effect* fx = mSkeletonMaterial->getEffect();
	if (NULL == fx)
	{
		return;
	}
	fx->setMatrix(name, pMatrix);
}
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

float Geometry::m_speed(1.0f);
