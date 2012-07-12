#include "Geometry.h"
#include "global.h"
Geometry::Geometry()
{
	_clear();
}
Geometry::~Geometry()
{

}
void Geometry::setMaterial(const std::string& fileName, Euclid::eMaterialType mt)
{
	mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(mt);
	mMaterial->setEffect(fileName);
	mMaterial->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureBoneWeight);
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
	static Euclid::AnimationTime at;
	at.start = 0;
	at.end = 1999;
	at.current += delta * 1.f;
	while(at.current >= at.end)
	{
		at.current -= at.end;
	}
	if (mSkin && mSkeleton)
	{
		mSkeleton->update(at, *mSkin);
	}
}
void Geometry::decode(const std::string& fileName)
{
	//
	setMesh(fileName);
	//
	setMaterial("shader/aPSkin.fx", Euclid::eMaterialType_Vertex);
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
	if (NULL == mSkeleton)
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
	std::vector<Mat4> matrices;
	for(Euclid::BoneIDReferenceMap::iterator it = mMesh->_bones.begin(); it != mMesh->_bones.end(); ++it)
	{
		if (it->first < 255)
		{
			matrices.push_back(mSkeleton->_matrices[it->first]);
		}
	}
	
	fx->setMatrixArray("gMatrixPalette", &matrices[0], matrices.size()); 
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
