#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlframe.h>
#include <atlsplit.h>
#include <atlddx.h>
#include "resource.h"
#include "renderImp.h"
#include <stack>
#include <string>
#include <cctype>
#include "base/ConstBaseIterator.h"
#include "tinyXML2/tinyxml2.h"
// idle handler
class IdleHandler
{
#define InterfaceIdleHandler(terminal)\
	public:\
	virtual void onIdle() ##terminal

#define IdleHandler_Base InterfaceIdleHandler(=0;)
#define IdleHandler_Derived InterfaceIdleHandler(;)
	IdleHandler_Base
};
//必须定义这个全局变量，否则非法
CAppModule _Module;
//
struct sAnimation
{
	std::string name;
	int			duration;
	std::string skinFile;
};
typedef std::map<std::string, sAnimation> AniMap;
typedef	ConstBaseIterator<AniMap>	AniMapIterator;
class IPrimitive
{
#define IPrimitiveInterface(terminal)\
public:\
	virtual void decode(const std::string& fileName) ##terminal\
	virtual void render() ##terminal\
	virtual void destroy() ##terminal\
	virtual void update(u32 delta) ##terminal\
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix ) ##terminal

#define IPrimitive_Base IPrimitiveInterface(=0;)
#define IPrimitive_Derived IPrimitiveInterface(;)
	IPrimitive_Base
public:
	virtual ~IPrimitive(){};
	
};
class Geometry: public IPrimitive
{
public:
	Geometry()
	{
		_clear();
	}
	~Geometry()
	{

	}
	
	IPrimitive_Derived

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
		mSkin = new Euclid::Skin;
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
		std::string parentPath = Buddha::FileSystem::getInstancePtr()->getParent(fileName);
		parentPath = Buddha::FileSystem::getInstancePtr()->getParent(parentPath);
		setSkin(parentPath + "/" + mAnimations.begin()->second.skinFile);
		mAniTime.current = 0;
		mAniTime.start = 0;
		mAniTime.end = mAnimations.begin()->second.duration;
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
		else
		{
			Error("no such file");
		}
		//
	}
	void Geometry::setSkeleton(const std::string& fileName)
	{
		mSkeleton = new Euclid::Skeleton;
		mSkeleton->create(fileName);
	}

	void setMaterial(const std::string& fileName)
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
		mMaterial->_zEnable = (Euclid::eZBufferType)r->IntAttribute("zEnable");
		mMaterial->_zWriteEnable = r->BoolAttribute("zWriteEnable");
		mMaterial->_alphaTestEnable = r->BoolAttribute("alphaTestEnable");
		mMaterial->_srcBlend = (Euclid::eBlend)r->IntAttribute("srcBlend");
		mMaterial->_destBlend = (Euclid::eBlend)r->IntAttribute("destBlend");
		mMaterial->mAlphaCmpFunc = (Euclid::eCmpFunc)r->IntAttribute("cmpFunc");
		mMaterial->mAlphaRef = r->IntAttribute("alphaRef");
		mMaterial->mDiffuse.setRGBA(r->IntAttribute("diffuse"));
		mMaterial->mUseVertexColor = r->BoolAttribute("useVertexColor");
		//
		{
			std::string aniPath = Buddha::FileSystem::getInstancePtr()->removeFileExtension(fileName) + ".ma";
			mMaterial->loadAnimation(aniPath);
		}
	}

private:
	void _clear()
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

public:
	std::string mName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
	Euclid::Skeleton* mSkeleton;
	Euclid::IMaterial* mSkeletonMaterial;
	Euclid::Skin* mSkin;
	AniMap mAnimations;
	Euclid::AnimationTime mAniTime;
	bool mHardwareSkin;
	Mat4 mUVMatrix; 
	bool mStopAimation;
	static float m_speed;
};
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

class SubEntity : public IPrimitive
{
public:
	SubEntity()
	{
		_clear();
	}
	~SubEntity()
	{
	
	}

	IPrimitive_Derived
private:
	void _clear()
	{
		mName.clear();
		mGeometry = NULL;
		mAnimations.clear();
	}
private:
	std::string mName;
	Geometry* mGeometry;
	AniMap mAnimations;
};
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

class Entity : public IPrimitive
{
public:
	Entity()
	{
		_clear();
	}
	~Entity()
	{

	}
	IPrimitive_Derived

private:
	void _clear()
	{
		mName.clear();
		mSubEntities.clear();
	}
private:
	std::string mName;
	typedef std::vector<SubEntity*> SubEntityVec;
	SubEntityVec mSubEntities;
};
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

//
class CViewWindow: 
	public CWindowImpl<CViewWindow>
	,public IdleHandler
{
public:
	BEGIN_MSG_MAP(CViewWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_SIZE, onSize)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CViewWindow()
	{
		_clear();
	}
public:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		new RenderEngineImp(m_hWnd);
		//
		return 1;
	}
	LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) 
	{
		_painted = true;
		b = FALSE;
		if (!RenderEngineImp::getInstancePtr()->isInitialized())
		{
			RenderEngineImp::getInstancePtr()->create();
			_create();
		}

		//
		return 1;
	}
	LRESULT onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) 
	{
		b = TRUE;
		if (RenderEngineImp::isNull())
		{
			return 1;
		}
		if (NULL == RenderEngineImp::getInstancePtr()->getRenderEngine())
		{
			RenderEngineImp::getInstancePtr()->_width = LOWORD(lParam);
			RenderEngineImp::getInstancePtr()->_height = HIWORD(lParam);
			return 1;
		}

		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onInvalidateDevice();
		//
		Euclid::sViewPort vp;
		vp.X = 0;
		vp.Y = 0;
		vp.Width = LOWORD(lParam);
		vp.Height = HIWORD(lParam);
		vp.MinZ = 0;
		vp.MaxZ = 1;
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setViewport(&vp);
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice(); 
		//
		return 1;
	}
	LRESULT onDestroy(UINT, WPARAM, LPARAM, BOOL&) 
	{
		if (RenderEngineImp::getInstancePtr()->isInitialized())
		{
			if (_material)
			{
				_material->destroy();
				delete _material;
				_material = 0;
			}
			//
			if (_vb)
			{
				_vb->destroy();
				delete _vb;
				_vb = NULL;
			}
			_destroymodel();
			RenderEngineImp::getInstancePtr()->destroy();
			delete RenderEngineImp::getInstancePtr();
		}

		//
		return 1;
	}
	void _create()
	{
		//
		_material = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		_material->setEffect("shader\\Position.fx");
		//
		Euclid::sPosition vertices[3];
		vertices[0].position = Vec3(-1.0f, 0.0f, 0.0f);
		vertices[1].position = Vec3(0.0f, 1.0f, 0.0f);
		vertices[2].position = Vec3(1.0f, 0.0f, 0.0f);
		_vb = RenderEngineImp::getInstancePtr()->getRenderEngine()->getBufferManager()->createVertexBuffer(3 * _material->getStride(), Euclid::eUsage_WriteOnly, Euclid::ePool_Manager);
		void* data = _vb->lock(0, 0, Euclid::eLock_Null);
		memcpy(data, vertices, 3 * _material->getStride());
		_vb->unLock();
		//
		//
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 16, Euclid::eFontProperty_Normal, "freeNormal");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
		RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(std::string("freetype\\LuYaHeiMb.TTF"), 28, Euclid::eFontProperty_BG_1, "freeBG1");
		_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
		_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
		_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));
		//
		
	}
	void _render()
	{
		_renderModel();
		_renderFont();
	}
	void _renderModel()
	{
		if (_model)
		{
			_model->render();
		}
		return;
		Euclid::Effect* fx = _material->getEffect();
		if (NULL == fx)
		{
			return;
		}
		_material->apply();
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);

			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vb, 0, _material->getStride());
			RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitive(Euclid::ePrimitive_TriangleList, 0, 1);

			fx->endPass();
		}
		fx->end(); 
	}
	void _renderFont()
	{
		// 文字最后画
		if (_font[Euclid::eFontProperty_Normal])
		{
			std::ostringstream ss;
			ss<<"FPS = "<<int(_fps);
			_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
		}
	}
	void _clear()
	{
		_painted = false;
		_material = NULL;
		_vb = NULL;
		for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
		{
			_font[i] = NULL;
		}
		_fps = 0.0f;
		_clearModel();
		_camera.setPosition(Vec3(0.0f, 50.0f, 150.0f));
	}
	void _calcFPS()
	{
		static float last_time = (float)timeGetTime();
		float current_time = (float)timeGetTime();
		static int count_frames = 0;
		++count_frames;
		float delta = current_time - last_time;
		// calculate FPS per Second
		if(delta >= 1000.0f)
		{
			_fps = (float)count_frames * 1000.0f  / delta;
			last_time = current_time;
			count_frames = 0;
		}
	}
	void _clearModel()
	{
		_modelFileName.clear();
		_model = NULL;
	}
	void _destroymodel()
	{
		if (_model)
		{
			_model->destroy();
			delete _model;
			_model = NULL;
		}
	}


	void changeModelFile( const std::string& fileName )
	{
		size_t dotPos = fileName.find('.');
		if (dotPos == std::string::npos)
		{
			return;
		}
		std::string suffix = fileName.substr(dotPos, fileName.size() - dotPos);
		std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::tolower);
		if(suffix == ".subentity")
		{
			_destroymodel();
			_clearModel();
			_model = new SubEntity;
		}
		else if(suffix == ".entity")
		{
			_destroymodel();
			_clearModel();
			_model = new Entity;
		}
		else
		{
			return;
		}
		std::string name = Buddha::FileSystem::getInstancePtr()->standardFilePath(fileName);	
 		_model->decode(name);
 		//_createModel();
// 		for (size_t i = 0; i != _observers.size(); ++i)
// 		{
// 			_observers[i]->update(_model);
// 		}
	}
private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
	IPrimitive*				_model;
	std::string				_modelFileName;
	Euclid::Camera			_camera;
	bool					_painted;
		
};

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BUTTON_OK, BN_CLICKED, OnBnClickedButtonOk)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
public:
	LRESULT OnBnClickedButtonOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		EndDialog(0);
		return 0;
	}
};
class CGeometryDlg : public CDialogImpl<CGeometryDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_GEOMETRY};

	BEGIN_MSG_MAP(CGeometryDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};
class CMaterialDlg : public CDialogImpl<CMaterialDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_MATERIAL};

	BEGIN_MSG_MAP(CMaterialDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
};

class CPropertyDlg : public CDialogImpl<CPropertyDlg>
{
public:
	enum { IDD = IDD_FORMVIEW_PROPERTY };

	BEGIN_MSG_MAP(CPropertyDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_TAB_PROPERTY, TCN_SELCHANGE, OnTcnSelchangeTabProperty)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//
		mTab = GetDlgItem(IDC_TAB_PROPERTY);
		mTab.InsertItem(0, TEXT("Geometry"));
		mTab.InsertItem(1, TEXT("Material"));
		mGeometryDlg.Create(mTab);
		mMaterialDlg.Create(mTab);
		//
		mTab.SetCurSel(0);
		mGeometryDlg.ShowWindow(SW_SHOW);
		mMaterialDlg.ShowWindow(SW_HIDE);
		//
		CenterWindow();
		ShowWindow(SW_NORMAL);
		return TRUE;
	}
	LRESULT CPropertyDlg::OnTcnSelchangeTabProperty(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		switch(mTab.GetCurSel())
		{
		case 0:
			{
				mGeometryDlg.ShowWindow(SW_SHOW);
				mMaterialDlg.ShowWindow(SW_HIDE);
			}
			break;
		case 1:
			{
				mGeometryDlg.ShowWindow(SW_HIDE);
				mMaterialDlg.ShowWindow(SW_SHOW);
			}
			break;


		}
		return 0;
	}

private:
	CTabCtrl mTab;
	CGeometryDlg mGeometryDlg;
	CMaterialDlg mMaterialDlg;
};
class CTreeDlg : public CDialogImpl<CTreeDlg>
{
private:
	CTreeViewCtrlEx m_tvCtrl;
public:
	enum { IDD = IDD_FORMVIEW };

	BEGIN_MSG_MAP(CTreeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		FORWARD_NOTIFICATIONS()// 把树状文件点击事件传给父窗口
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow();
		ShowWindow(SW_NORMAL);
		//
		m_tvCtrl = GetDlgItem(IDC_TREE);
		//
		{
			CTreeItem t;
			CTreeItem tail;
			
			//如何插入子节点
			{
				HANDLE hFind = INVALID_HANDLE_VALUE;
				std::string dataPath = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
				//如何插入根节点
				{
					t = m_tvCtrl.InsertItem ( NULL, TVI_ROOT, TVI_LAST);
					t.SetText(dataPath.c_str());
				}
				std::stack<std::string> pathes;
				std::stack<CTreeItem> items;
				pathes.push(dataPath);
				items.push(t);
				
				WIN32_FIND_DATA ffd;
				while(!pathes.empty())
				{
					t = items.top();
					items.pop();
					dataPath = pathes.top();
					pathes.pop();
					hFind = FindFirstFile((dataPath +  + "/*").c_str(), &ffd);
					do
					{
						std::string name = ffd.cFileName;
						if(name != "." && name != ".." )
						{
							if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								pathes.push(dataPath + "/" + ffd.cFileName);
								tail = t.AddTail(ffd.cFileName, 0);
								items.push(tail);
							}
							else
							{
								//if (name.find(".entity") != std::string::npos)
								{
									t.AddTail(ffd.cFileName, 0);
								}
							}
						}
					}while (FindNextFile(hFind, &ffd) != 0);
				}
				
			}
		}
		return TRUE;
	}
};

class CMainWindow : 
	public CFrameWindowImpl<CMainWindow>
	,public IdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("ModelFrameWindowClass"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)
		NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnTVSelChanged) 
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CMainWindow()
	{

	}
public:
		LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) 
		{
			b = false;
			return 0;
		}
private:
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) 
	{
		//
		CreateSimpleToolBar();

		//
		//CreateSimpleStatusBar(TEXT("Ready"));
		m_hWndStatusBar = m_wndStatusBar.Create ( *this );
		// Create the status bar panes.
		int anPanes[] = { ID_DEFAULT_PANE, ID_DEFAULT_PANE_1, 
			ID_DEFAULT_PANE_2 };

		m_wndStatusBar.SetPanes ( anPanes, 3, true );

		//
		m_wndSplitVertical.Create ( *this, rcDefault );
	
		//
		mTreeDlg.Create(m_wndSplitVertical, rcDefault);
		m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_LEFT, mTreeDlg);
		//
		m_wndSplitVerticalProperty.Create(m_wndSplitVertical, rcDefault);
		m_wndSplitVertical.SetSplitterPane(SPLIT_PANE_RIGHT, m_wndSplitVerticalProperty);
		//
		mViewWindow.Create(m_wndSplitVerticalProperty, rcDefault);
		m_wndSplitVerticalProperty.SetSplitterPane(SPLIT_PANE_LEFT, mViewWindow);
		//
		mPropertyDlg.Create(m_wndSplitVerticalProperty, rcDefault);
		m_wndSplitVerticalProperty.SetSplitterPane(SPLIT_PANE_RIGHT, mPropertyDlg);
		//
		m_hWndClient = m_wndSplitVertical;
		UpdateLayout();
		/*
			Note that you need to set m_hWndClient and call CFrameWindowImpl::UpdateLayout() before setting the splitter position. 
			UpdateLayout() resizes the splitter window to its initial size. 
			If you skip that step, the splitter's size isn't under your control and it might be smaller than 200 pixels wide. 
			The end result would be that SetSplitterPos() wouldn't have the effect you wanted.
		*/
		m_wndSplitVertical.SetSplitterPos(200);
		m_wndSplitVerticalProperty.SetSplitterPos(700);
		//
		return 1;
	}
	LRESULT OnHelpAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加命令处理程序代码
		CAboutDlg ad;
		ad.DoModal();
		return 0;
	}

	LRESULT OnTVSelChanged(int, LPNMHDR pnmh, BOOL&)
	{
		// 如何解析参数
		LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pnmh);
		CTreeViewCtrlEx tree = pnmh->hwndFrom;
		if (tree.IsWindow())
		{
			CTreeItem item = tree.GetSelectedItem();
			std::string path;
			for (; !item.IsNull(); item = item.GetParent())
			{
				std::string name(256, 0);
				item.GetText((LPTSTR)name.c_str(), 256);
				std::string temp = name.c_str();
				if (!path.empty())
				{
					path = temp + "/" + path;
				}
				else
				{
					path = temp;
				}
			}
			//
			m_wndStatusBar.SetPaneText(ID_DEFAULT_PANE, TEXT(path.c_str()));
			//
			mViewWindow.changeModelFile(path);
		}

		return 0L;
	}

private:
	CViewWindow mViewWindow;	
	CTreeDlg mTreeDlg;
	CPropertyDlg mPropertyDlg;
	CSplitterWindow m_wndSplitVertical;
	CSplitterWindow m_wndSplitVerticalProperty;
	CMultiPaneStatusBarCtrl m_wndStatusBar;
};
void CViewWindow::onIdle()
{
	if (RenderEngineImp::isNull()) 
	{
		return;
	}
	static u32 oldTime = GetTickCount();
	u32 currentTime = GetTickCount();
	float delta = currentTime - oldTime;
	oldTime = currentTime;
	_camera.update();
	if (_model)
	{
		Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix();
		_model->setMatrix("gWorldViewProj", m);
		_model->setMatrix("gView", _camera.getViewMatrix());
		_model->setMatrix("gProjection", _camera.getProjectionMatrix());
		//
		_model->update(delta);
	}

	_calcFPS();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, Euclid::Color::Black, 1.0f, 0L);
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_render();
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);
}
void CMainWindow::onIdle()
{
	mViewWindow.onIdle();
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	CMainWindow mw;
	mw.CreateEx();
	//如果这里SW_MAXIMIZE，PIX不能正常工作
	mw.ShowWindow(nCmdShow);
	mw.UpdateWindow();
	mw.ShowWindow(SW_MAXIMIZE);
	MSG m;
	::ZeroMemory(&m, sizeof(m));
	for (;;)
	{
		::ZeroMemory(&m, sizeof(m));
		if (::PeekMessage(&m, NULL, 0, 0, PM_NOREMOVE))
		{
			::GetMessage(&m, NULL, 0, 0);
			if (m.message == WM_QUIT)
			{
				break;
			}
			::TranslateMessage(&m);
			::DispatchMessage(&m);
		}
		else
		{
			mw.onIdle();
		}
	}
		
	return 0;
}